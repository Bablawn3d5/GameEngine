// Copyright 2015 Babalawn3d5

#pragma once

#include <entityx/entityx.h>

struct Stats {
 public:
   explicit Stats(float speed = 0.0f, float maxHp = 100.0f, float currentHp = 100.0f)
      : speed(speed), maxHp(maxHp), currentHp(currentHp) {}
   ~Stats() {}

   float speed;
   float maxHp;
   float currentHp;
   bool godmode = false;
};
