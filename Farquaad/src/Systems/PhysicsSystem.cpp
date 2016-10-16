// Copyright 2015 Bablawn3d5
#include <Farquaad/Systems/PhysicsSystem.h>
#include <Farquaad/Components.hpp>
#include <SFML/Graphics.hpp>
#include <cmath>

namespace ex = entityx;

void EntityXPhysicsContactListener::BeginContact(b2Contact* contact) {
  ex::Entity entityA = *(static_cast<ex::Entity*>(contact->GetFixtureA()->GetBody()->GetUserData()));
  ex::Entity entityB = *(static_cast<ex::Entity*>(contact->GetFixtureB()->GetBody()->GetUserData()));
#ifdef DEBUG
  auto indexA = entityA.id().index();
  auto indexB = entityB.id().index();
#endif // DEBUG
  // Assuming box2d will behave well and only emit one BeginContact per collision between
  // two objects.
  auto physicsA = entityA.component<Physics>().get();
  physicsA->collidingWithSet.push_back(entityB);
  physicsA->isColliding = true;
  physicsA->collisionCount = physicsA->collidingWithSet.size();

  auto physicsB = entityB.component<Physics>().get();
  physicsB->collidingWithSet.push_back(entityA);
  physicsB->isColliding = true;
  physicsB->collisionCount = physicsB->collidingWithSet.size();
}

void EntityXPhysicsContactListener::EndContact(b2Contact* contact) {
  ex::Entity entityA = *(static_cast<ex::Entity*>(contact->GetFixtureA()->GetBody()->GetUserData()));
  ex::Entity entityB = *(static_cast<ex::Entity*>(contact->GetFixtureB()->GetBody()->GetUserData()));
#ifdef DEBUG
  auto indexA = entityA.id().index();
  auto indexB = entityB.id().index();
#endif // DEBUG
  // Assuming EndContact is always called when collision ends.
  auto physicsA = entityA.component<Physics>();
  auto elemB = std::find(physicsA->collidingWithSet.begin(), physicsA->collidingWithSet.end(), entityB);
  assert(elemB != physicsA->collidingWithSet.end());
  physicsA->collidingWithSet.erase(elemB);
  physicsA->collisionCount = physicsA->collidingWithSet.size();

  auto physicsB = entityB.component<Physics>();
  auto elemA = std::find(physicsB->collidingWithSet.begin(), physicsB->collidingWithSet.end(), entityA);
  assert(elemA != physicsB->collidingWithSet.end());
  physicsB->collidingWithSet.erase(elemA);
  physicsB->collisionCount = physicsB->collidingWithSet.size();

  if ( physicsA->collidingWithSet.size() == 0 ) { physicsA->isColliding = false; }
  if ( physicsB->collidingWithSet.size() == 0 ) { physicsB->isColliding = false; }
}

void PhysicsSystem::update(ex::EntityManager &em, ex::EventManager &events, ex::TimeDelta dt) {
    // Resync physics components that are 'dirty'.
    em.each<Physics>(
        [this](ex::Entity entity, Physics& physics) {
        if ( physics.isDirty == false ) {
            return;
        }

        physics.isDirty = false;
        physics.halfSize = 0.5f * (sf::Vector2f)physics.size * METERS_PER_PIXEL;
        if ( entity.has_component<Body>() ) {
          auto body = entity.component<Body>().get();
          sf::Vector2f physicsBodyPos = (body->position * METERS_PER_PIXEL) + physics.halfSize;
          auto rotation = 0.f;
          if ( physics.body != nullptr ) {
            // Assuming one fixture per-physics body here.
            physics.body->DestroyFixture(physics.body->GetFixtureList());
            rotation = physics.body->GetAngle();
          }

          // Resync Fixture
          b2PolygonShape shape;
          shape.SetAsBox(physics.halfSize.x, physics.halfSize.y);
          b2FixtureDef shapeDef;
          shapeDef.shape = &shape;
          shapeDef.density = 1.0f;
          shapeDef.restitution = 0.0f;
          shapeDef.friction = 0.0f;
          shapeDef.filter.categoryBits = physics.collisionCategory;
          shapeDef.filter.maskBits = physics.collisionMask;
          shapeDef.filter.groupIndex = physics.collisionGroup;
          shapeDef.isSensor = physics.isSensor;
          b2BodyDef bodyDef;
          bodyDef.type = physics.bodyType;
          bodyDef.bullet = physics.isBullet;
          b2Body * boxbody = physicsWorld->CreateBody(&bodyDef);
          boxbody->CreateFixture(&shapeDef);
          physics.self = entity;
          boxbody->SetUserData(static_cast<void*>(&physics.self));

          // TODO(SMA) : Sync Rotation.
          boxbody->SetTransform( {physicsBodyPos.x, physicsBodyPos.y}, rotation);
          physics.body = boxbody;

          // HACK(SMA) : Assign it a rectangle if there's no drawable component.
          // Really should remove me now that we've got the physics debugger.
          if ( !entity.has_component<Renderable>() ) {
            auto renderable = entity.assign<Renderable>();
            auto rect_ptr = std::make_shared<sf::RectangleShape>((sf::Vector2f)physics.size);
            renderable->drawable = rect_ptr;
            renderable->transform = rect_ptr;
            renderable->font_string = "Debug Shape";
            renderable->isDirty = false;
          }

          physics.collidingWithSet.clear();
          physics.isColliding = false;
          physics.collisionCount = 0;
        }

        // TODO(SMA) : Resync stat changes
        if ( entity.has_component<Stats>() ) {
        }
    });

    // Update velocity in the physics world.
    em.each<Body, Physics, Stats>(
        [this](ex::Entity entity, Body &body, Physics& physics, Stats& stats) {
        // Something has gone horribly wrong.
        assert(physics.body != NULL);

        sf::Vector2f direction = body.direction;
        float normalization_factor = (direction.x * direction.x) + (direction.y * direction.y);
        if ( fabs(normalization_factor - 0.0f) > FLT_EPSILON ) {
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

        for ( auto it = physics.collidingWithSet.begin(); it != physics.collidingWithSet.end();) {
          if ( !it->valid() ) { it = physics.collidingWithSet.erase(it); }
          else  { it++; }
        }
    });

    // Run the simulation
    // Maximum number of steps, to avoid degrading to an halt.
    const int MAX_STEPS = 5;

    fixedTimestepAccumulator += static_cast<float>(dt);
    const int nSteps = static_cast<int> (
      std::floor(fixedTimestepAccumulator / FIXED_TIMESTEP)
    );
    // To avoid rounding errors, touches fixedTimestepAccumulator_ only
    // if needed.
    if ( nSteps > 0 ) {
      fixedTimestepAccumulator -= nSteps * FIXED_TIMESTEP;
    }

    assert(
      "Accumulator must have a value lesser than the fixed time step" &&
      fixedTimestepAccumulator < FIXED_TIMESTEP + FLT_EPSILON
    );
    fixedTimestepAccumulatorRatio = fixedTimestepAccumulator / FIXED_TIMESTEP;

    // This is similar to clamp "dt":
    //    dt = std::min (dt, MAX_STEPS * FIXED_TIMESTEP)
    const int nStepsClamped = std::min(nSteps, MAX_STEPS);
    for ( int i = 0; i < nStepsClamped; ++i ) {
      physicsWorld->Step(FIXED_TIMESTEP, VELOCITY_ITERATIONS, POSITION_ITERATIONS);
    }

    physicsWorld->ClearForces();

    // Update Entities based on their position in the physics world
    em.each<Body, Physics, Stats>(
        [this](ex::Entity entity, Body &body, Physics& physics, Stats& stats) {
        b2Vec2 physicsPosition = physics.body->GetPosition();

        body.position.x = (physicsPosition.x - physics.halfSize.x) * PIXELS_PER_METER;
        body.position.y = (physicsPosition.y - physics.halfSize.y) * PIXELS_PER_METER;
    });
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
