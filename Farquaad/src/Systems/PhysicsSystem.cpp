// Copyright 2015 Bablawn3d5
#include <Farquaad/Systems/PhysicsSystem.h>
#include <Farquaad/Components.hpp>
#include <SFML/Graphics.hpp>
#include <cmath>

namespace ex = entityx;

void PhysicsSystem::update(ex::EntityManager &em, ex::EventManager &events, ex::TimeDelta dt) {
    // Resync physics components that are 'dirty'.
    em.each<Physics>(
        [this](ex::Entity entity, Physics& physics) {
        if ( physics.isDirty == false ) {
            return;
        }

        physics.isDirty = false;
        physics.halfSize = 0.5f * (sf::Vector2f)physics.size;

        if ( physics.body == NULL ) {
            return;
        }

        if ( entity.has_component<Body>() ) {
          auto body = entity.component<Body>().get();
          sf::Vector2f physicsBodyPos = (body->position * METERS_PER_PIXEL) + physics.halfSize;
          auto rotation = physics.body->GetAngle();
          // TODO(SMA) : Sync Rotation.
          physics.body->SetTransform( {physicsBodyPos.x, physicsBodyPos.y}, rotation);
        }

        // TODO(SMA) : Resync stat changes
        if ( entity.has_component<Stats>() ) {
        }
    });

    for ( auto e : entitiesToCreatePhysicsFor ) {
        // Intialize physics components to new Entities
        if ( e.has_component<Physics>() &&
            e.has_component<Body>() &&
            e.has_component<Stats>() ) {
            auto physics = e.component<Physics>().get();
            auto body = e.component<Body>().get();

            // Something has gone horribly wrong.
            assert(physics->body == NULL);

            // We don't process an entity unless it has all the required components
            // TODO(MRC): Look into Component dependencies

            physics->halfSize = 0.5f * (sf::Vector2f)physics->size;

            b2BodyDef bodyDef;
            bodyDef.type = physics->bodyType;

            sf::Vector2f physicsBodyPos = (body->position * METERS_PER_PIXEL) + physics->halfSize;
            bodyDef.position.Set(physicsBodyPos.x, physicsBodyPos.y);
            bodyDef.bullet = true;
            b2Body * boxbody = physicsWorld->CreateBody(&bodyDef);

            b2PolygonShape shape;
            shape.SetAsBox(physics->halfSize.x, physics->halfSize.y);
            b2FixtureDef shapeDef;
            shapeDef.shape = &shape;
            shapeDef.density = 10.0;
            shapeDef.restitution = 0.0f;
            shapeDef.friction = 0.0f;
            boxbody->CreateFixture(&shapeDef);

            physics->body = boxbody;

            // Assign it a rectangle if there's no renderable component.
            if ( !e.has_component<RenderableShape>() ) {
                e.assign<RenderableShape>(new sf::RectangleShape((sf::Vector2f)physics->size));
            }
        } else {
            continue;
        }
    }

    entitiesToCreatePhysicsFor.clear();

    // Update velocity in the physics world.
    em.each<Body, Physics, Stats>(
        [this](ex::Entity entity, Body &body, Physics& physics, Stats& stats) {
        // Something has gone horribly wrong.
        assert(physics.body != NULL);

        sf::Vector2f direction = body.direction;
        float normalization_factor = (direction.x * direction.x) + (direction.y * direction.y);
        if ( !(fabs(normalization_factor - 0.0f) < b2_epsilon) ) {
            normalization_factor = 1.0f / sqrt(normalization_factor);

            float x_direction_normalized = direction.x * normalization_factor;
            float y_direction_normalized = direction.y * normalization_factor;

            b2Vec2 vel = physics.body->GetLinearVelocity();
            vel.x = x_direction_normalized * stats.speed * METERS_PER_PIXEL;
            vel.y = y_direction_normalized * stats.speed * METERS_PER_PIXEL;
            physics.body->SetLinearVelocity(vel);
        } else {
            b2Vec2 vel = physics.body->GetLinearVelocity();
            vel.x = 0.0f;
            vel.y = 0.0f;
            physics.body->SetLinearVelocity(vel);
        }
    });

    // Run the simulation
    physicsWorld->Step(static_cast<float>(dt), VELOCITY_ITERATIONS, POSITION_ITERATIONS);

    // Update Entities based on their position in the physics world
    em.each<Body, Physics, Stats>(
        [this](ex::Entity entity, Body &body, Physics& physics, Stats& stats) {
        b2Vec2 physicsPosition = physics.body->GetPosition();

        body.position.x = (physicsPosition.x - physics.halfSize.x) * PIXELS_PER_METER;
        body.position.y = (physicsPosition.y - physics.halfSize.y) * PIXELS_PER_METER;
    });

    // TODO(SMA) : Implement me
    // Entity entityA;
    // Entity entityB;

    // // Process any collisions that might have happened during the simulation
    // for each (EntityContact contact in contactListener->contacts) {
    //     for ( auto& entity : em.entities_with_components(physics) ) {
    //         if ( physics->body == contact.fixtureA->GetBody() ) {
    //             entityA = entity;
    //         } else if ( physics->body == contact.fixtureB->GetBody() ) {
    //             entityB = entity;
    //         }

    //         if ( entityA != NULL && entityB != NULL ) {
    //             break;
    //         }
    //     }
    // }

    // if ( entityA != NULL && entityB != NULL ) {
    //     events.emit<CollisionEvent>(entityA, entityB);
    // }
}

void PhysicsSystem::receive(const ex::ComponentAddedEvent<Physics> &componentAddedEvent) {
    entitiesToCreatePhysicsFor.push_back(componentAddedEvent.entity);
}

void PhysicsSystem::receive(const ex::EntityDestroyedEvent &entityDestroyedEvent) {
    ex::Entity entity = entityDestroyedEvent.entity;
    if ( entity.has_component<Physics>() ) {
        auto physics = entity.component<Physics>();
        physicsWorld->DestroyBody(physics->body);
        // Remove physics component as its invalid now.
        physics.remove();
    }
}
