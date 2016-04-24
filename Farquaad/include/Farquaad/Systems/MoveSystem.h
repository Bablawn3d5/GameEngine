// Copyright 2016 Bablawn3d5

#pragma once

#include <entityx/entityx.h>
#include <functional>
#include <map>
#include <string>

namespace ex = entityx;

struct MoveSystem : public ex::System<MoveSystem> {
public:
  typedef std::map<const std::string, std::function<void(ex::Entity)>>  KeyBindReponder;
  explicit MoveSystem();
  void update(ex::EntityManager &em, ex::EventManager &events, ex::TimeDelta dt);  // NOLINT
private:
  KeyBindReponder responders;
};
