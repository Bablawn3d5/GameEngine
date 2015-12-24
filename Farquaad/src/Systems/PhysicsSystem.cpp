// Copyright 2015 Bablawn3d5
#include <Farquaad/Systems/RenderSystem.h>
#include <Farquaad/Components.hpp>

void RenderSystem::update(ex::EntityManager & em,
                          ex::EventManager & events, ex::TimeDelta dt) {
    em.each<Body, RenderableShape>(
        [this](ex::Entity entity, Body &body, RenderableShape &renderable) {
    });
}
