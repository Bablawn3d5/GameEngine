// Copyright 2015 Bablawn3d5

#pragma once

#include <entityx/entityx.h>
#include <SFML/System/Vector2.hpp>
#include <vector>

namespace ex = entityx;

struct InputResponder {
    std::vector<std::string> responds;
    sf::Vector2i mousePos;
    sf::Vector2i mouseGamePos;
};