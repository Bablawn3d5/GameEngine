// Copyright 2016 Bablawn3d5

#pragma once

#include <entityx/entityx.h>

namespace ex = entityx;

struct DestroyerSystem : public ex::System<DestroyerSystem> {
public:
  explicit DestroyerSystem();
  void update(ex::EntityManager &em, ex::EventManager &events, ex::TimeDelta dt);  // NOLINT
};
