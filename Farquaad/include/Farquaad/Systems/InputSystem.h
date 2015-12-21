// Copyright 2015 Babalawn3d5

#pragma once

#include <entityx\entityx.h>

namespace ex = entityx;

struct InputSystem : public ex::System<InputSystem> {
   void update(ex::EntityManager &em, ex::EventManager &events, ex::TimeDelta dt);
};
