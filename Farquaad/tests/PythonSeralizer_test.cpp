// Copyright 2016 Bablawn3d5
#include <Farquaad/Serialization.hpp>
#include <entityx/entityx.h>
#include <string>
#include <sstream>
#include <iostream>
#include <catch.hpp>

using entityx::Entity;
using entityx::EventManager;
using entityx::EntityManager;

template <typename T>
int size(const T &t) {
    int n = 0;
    for ( auto i : t ) {
        ++n;
        (void)i;  // Unused on purpose, suppress warning
    }
    return n;
}
