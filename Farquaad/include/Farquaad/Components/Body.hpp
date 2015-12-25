// Copyright 2015 Bablawn3d5

#pragma once

#include <entityx/entityx.h>
#include <SFML/System.hpp>

struct Body {
    explicit Body(sf::Vector2f position,
                  sf::Vector2f direction)
        : position(position), direction(direction) {
    }
    explicit Body(float px = 0.0f, float py = 0.0f,
                  float dx = 0.0f, float dy = 0.0f)
        : position(px, py), direction(dx, dy) {
    }
    ~Body() {}

    sf::Vector2f position;
    sf::Vector2f direction;
};
