// Copyright 2015 Babalawn3d5

#pragma once

#include <entityx\entityx.h>

struct Stats : Componenet`<Stats> {
 public:
   explicit Stats(float speed = 0.0f, float maxHp = 100.0f, float currentHp = 100.0f
      , bool godmode = false)
      : speed(speed), maxHp(maxHp), currentHp(currentHp), godmode(godmode) {}
   ~Stats() {}

   float speed;
   float maxHp;
   float currentHp;
   bool godmode;
};
