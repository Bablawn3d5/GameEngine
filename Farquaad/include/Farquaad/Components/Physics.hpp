// Copyright 2015 Bablawn3d5

#pragma once

#include <entityx/entityx.h>
#include <Box2D/Box2D.h>


enum CollisionGroup {
  GROUP_1 = 1 << 0,
  GROUP_2 = 1 << 1,
  GROUP_3 = 1 << 2,
  GROUP_4 = 1 << 3,
  GROUP_5 = 1 << 4,
  GROUP_6 = 1 << 5,
  GROUP_7 = 1 << 6,
  GROUP_8 = 1 << 7,
  GROUP_9 = 1 << 8,
  GROUP_10 = 1 << 9,
  GROUP_11 = 1 << 10,
  GROUP_12 = 1 << 11,
  GROUP_13 = 1 << 12,
  GROUP_14 = 1 << 13,
  GROUP_15 = 1 << 14,
  GROUP_NONE = -1
};

enum CollisionCategory {
  CATEGORY_1 = 1 << 0,
  CATEGORY_2 = 1 << 1,
  CATEGORY_3 = 1 << 2,
  CATEGORY_4 = 1 << 3,
  CATEGORY_5 = 1 << 4,
  CATEGORY_6 = 1 << 5,
  CATEGORY_7 = 1 << 6,
  CATEGORY_8 = 1 << 7,
  CATEGORY_9 = 1 << 8,
  CATEGORY_10 = 1 << 9,
  CATEGORY_11 = 1 << 10,
  CATEGORY_12 = 1 << 11,
  CATEGORY_13 = 1 << 12,
  CATEGORY_14 = 1 << 13,
  CATEGORY_15 = 1 << 14,
  CATEGORY_16 = 1 << 15
};

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
        isDirty = true;
    }
    ~Physics() {}

    bool isDirty;

    b2Body* body = NULL;
    b2BodyType bodyType;
    sf::Vector2f halfSize;
    sf::Vector2i size;

    CollisionCategory collisionCategory = CollisionCategory::CATEGORY_1;
    CollisionCategory collisionMask = CollisionCategory::CATEGORY_1;
    CollisionGroup collisionGroup = CollisionGroup::GROUP_1;
};
