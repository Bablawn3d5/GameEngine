// Copyright 2015 Bablawn3d5

#pragma once

#include <entityx/entityx.h>
#include <SFML/Window.hpp>
#include <Farquaad/Components.hpp>
#include <Box2D/Box2D.h>
#include <map>
#include <string>
#include <vector>

namespace ex = entityx;

struct PhysicsSystem : public ex::System<PhysicsSystem>,
    public ex::Receiver<PhysicsSystem> {
public:
    std::shared_ptr<b2World> physicsWorld;

    // TODO(SMA): Make me not hard-coded.
    const float FIXED_TIMESTEP = 1.f / 60.f;
    const float PIXELS_PER_METER = 2.f;
    const float METERS_PER_PIXEL = 1.f / PIXELS_PER_METER;

    explicit PhysicsSystem(std::shared_ptr<b2World> physicsWorld) :
        physicsWorld(physicsWorld), 
      fixedTimestepAccumulator(0),
      fixedTimestepAccumulatorRatio(0) {
        physicsWorld->SetAutoClearForces(false);
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
    float fixedTimestepAccumulator;
    float fixedTimestepAccumulatorRatio;

    const int VELOCITY_ITERATIONS = 8;
    const int POSITION_ITERATIONS = 4;
};
