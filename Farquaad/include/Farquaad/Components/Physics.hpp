// Copyright 2015 Bablawn3d5

#pragma once

#include <entityx/entityx.h>
#include <Box2D/Box2D.h>

struct Physics {
public:
    // Pass in the height/width as pixels
    explicit Physics(
        b2BodyType bodyType = b2_staticBody,
        int widthPixels = 0,
        int heightPixels = 0) :
        bodyType(bodyType),
        size({ widthPixels , heightPixels }) {
        halfSize = 0.5f * (sf::Vector2f)size;
    }
    ~Physics() {}

    b2Body* body = NULL;
    b2BodyType bodyType;
    sf::Vector2f halfSize;
    sf::Vector2i size;

    uint16 collosionCategory;
    uint16 collosionMask;
};
