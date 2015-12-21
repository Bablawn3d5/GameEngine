// Copyright 2015 Babalawn3d5

#pragma once

#include <entityx\entityx.h>

struct Body {
 public:
   explicit Body(int x = 0, int y = 0, float xDirection = 0.0f, float yDirection = 0.0f)
      : x(x), y(y), xDirection(xDirection), yDirection(yDirection) {}
   ~Body() {}

   int x, y;
   float xDirection, yDirection;
};

