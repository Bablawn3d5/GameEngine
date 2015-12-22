// Copyright 2015 Bablawn3d5

#pragma once

#include <entityx/entityx.h>

struct Stats {
    explicit Stats(float speed = 0.0f, float maxHp = 100.0f, float currentHp = 100.0f)
        : speed(speed), maxHp(maxHp), currentHp(currentHp) {
    }
    ~Stats() {}

    float speed;
    float maxHp;
    float currentHp;
    bool godmode = false;
};
