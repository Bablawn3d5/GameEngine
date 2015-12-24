// Copyright 2015 Bablawn3d5
#include <Farquaad/Systems/PhysicsSystem.h>
#include <Farquaad/Components.hpp>

void PhysicsSystem::update(ex::EntityManager & em,
                           ex::EventManager & events, ex::TimeDelta dt) {
    em.each<Body, RenderableShape>(
        [this](ex::Entity entity, Body &body, RenderableShape &renderable) {
    });
}
