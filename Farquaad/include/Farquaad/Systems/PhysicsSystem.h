// Copyright 2015 Bablawn3d5

#pragma once

#include <entityx/entityx.h>
#include <SFML/Window.hpp>
#include <Farquaad/Components.hpp>
#include <Farquaad/Box2D/SFMLB2DDebug.h>
#include <Box2D/Box2D.h>
#include <map>
#include <string>
#include <vector>

namespace ex = entityx;

struct PhysicsSystem : public ex::System<PhysicsSystem>,
    public ex::Receiver<PhysicsSystem> {
public:
    std::shared_ptr<b2World> physicsWorld;

    explicit PhysicsSystem(std::shared_ptr<b2World> physicsWorld,
                           SFMLB2DDebug* drawer = NULL) :
        physicsWorld(physicsWorld) {
        if ( physicsWorld != NULL && drawer != NULL )
            physicsWorld->SetDebugDraw(drawer);
    }
    ~PhysicsSystem() {}

    void receive(const ex::EntityDestroyedEvent &entityDestroyedEvent);
    void receive(const ex::ComponentAddedEvent<Physics> &componentAddedEvent);
    void configure(ex::EventManager &event_manager) {  // NOLINT
        event_manager.subscribe<ex::EntityDestroyedEvent>(*this);
        event_manager.subscribe<ex::ComponentAddedEvent<Physics>>(*this);
    }

    void update(ex::EntityManager &em, ex::EventManager &events, ex::TimeDelta dt);  // NOLINT

private:
    std::vector<ex::Entity> entitiesToCreatePhysicsFor;

    const float PIXELS_PER_METER = 1.0f;
    const float METERS_PER_PIXEL = 1.0f;
    const int VELOCITY_ITERATIONS = 8;
    const int POSITION_ITERATIONS = 4;
};
