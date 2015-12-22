// Copyright 2015 Babalawn3d5

#pragma once

#include <entityx/entityx.h>

namespace ex = entityx;

struct CollisionEvent {
 public:
   CollisionEvent(const ex::Entity& entity1, const ex::Entity& entity2) :
      entity1(entity1),
      entity2(entity2) {
   }

   ex::Entity entity1;
   ex::Entity entity2;
};

