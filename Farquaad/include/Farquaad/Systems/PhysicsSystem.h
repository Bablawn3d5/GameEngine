// Copyright 2015 Bablawn3d5

#pragma once

#include <entityx/entityx.h>
#include <SFML/Window.hpp>
#include <map>
#include <string>

namespace ex = entityx;

struct PhysicsSystem : public ex::System<PhysicsSystem> {
public:
    explicit PhysicsSystem(sf::Window &target) {}

    void update(ex::EntityManager &em, ex::EventManager &events, ex::TimeDelta dt);  // NOLINT
};
