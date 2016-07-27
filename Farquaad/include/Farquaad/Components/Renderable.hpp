// Copyright 2016 Bablawn3d5

#pragma once

#include <entityx/entityx.h>
#include <SFML/Graphics.hpp>
#include <string>

struct Renderable {
    std::string texture_name;
    std::string sprite_name;

    std::shared_ptr<sf::Drawable> drawable;
    std::weak_ptr<sf::Transformable> transform;
    // TODO(SMA): Animation data to feed into the renderer
    // std::weak_ptr<Animated> animation;
};
