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

class EntityXPhysicsContactListener : public b2ContactListener {
public:
  void BeginContact(b2Contact* contact);
  void EndContact(b2Contact* contact);
};

struct PhysicsSystem : public ex::System<PhysicsSystem>,
    public ex::Receiver<PhysicsSystem> {
public:
    std::shared_ptr<b2World> physicsWorld;
    EntityXPhysicsContactListener contactListener;

    // TODO(SMA): Make me not hard-coded.
    const float FIXED_TIMESTEP = 1.f / 60.f;
    const float PIXELS_PER_METER = 50.f;
    const float METERS_PER_PIXEL = 1.f / PIXELS_PER_METER;

    explicit PhysicsSystem(std::shared_ptr<b2World> physicsWorld) :
        physicsWorld(physicsWorld), 
      fixedTimestepAccumulator(0),
      fixedTimestepAccumulatorRatio(0) {
        physicsWorld->SetAutoClearForces(false);
        physicsWorld->SetContactListener(&contactListener);
    }
    ~PhysicsSystem() {}

    void receive(const ex::EntityDestroyedEvent &entityDestroyedEvent);
    void configure(ex::EventManager &event_manager) {  // NOLINT
        event_manager.subscribe<ex::EntityDestroyedEvent>(*this);
    }

    void update(ex::EntityManager &em, ex::EventManager &events, ex::TimeDelta dt);  // NOLINT
private:
    float fixedTimestepAccumulator;
    float fixedTimestepAccumulatorRatio;

    const int VELOCITY_ITERATIONS = 8;
    const int POSITION_ITERATIONS = 4;
};