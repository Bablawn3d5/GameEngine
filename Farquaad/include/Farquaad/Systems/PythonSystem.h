// Copyright 2015 Bablawn3d5

#pragma once

#include <entityx/entityx.h>

namespace ex = entityx;

struct PythonSystem : public ex::System<PythonSystem> {
public:
    explicit PythonSystem();
    ~PythonSystem();

    void update(ex::EntityManager &em, ex::EventManager &events, ex::TimeDelta dt);  // NOLINT
};
