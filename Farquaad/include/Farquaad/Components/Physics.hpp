// Copyright 2015 Bablawn3d5

#pragma once

#include <entityx/entityx.h>
#include <SFML/System.hpp>

struct Body {
    explicit Body(sf::Vector2f position = { 0.0f, 0.0f },
                  sf::Vector2f direction = { 0.0f, 0.0f })
        : position(position), direction(direction) {
    }
    explicit Body(float x, float y, float xDirection = 0.0f, float yDirection = 0.0f)
        : position(x, y), direction(xDirection, yDirection) {
    }
    ~Body() {}

    sf::Vector2f position;
    sf::Vector2f direction;
};
