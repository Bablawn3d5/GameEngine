// Copyright 2015 Bablawn3d5

#pragma once

#include <entityx/entityx.h>
#include <SFML/Graphics.hpp>

namespace ex = entityx;

struct RenderSystem : public ex::System<RenderSystem> {
    explicit RenderSystem(sf::RenderTarget &targ) : target(targ) {}

    void update(ex::EntityManager &em, ex::EventManager &events, ex::TimeDelta dt);  // NOLINT

    sf::RenderTarget& target;
};
