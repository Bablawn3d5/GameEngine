// Copyright 2015 Bablawn3d5

#pragma once

#include <entityx/entityx.h>
#include <SFML/Graphics.hpp>

struct RenderableShape {
    explicit RenderableShape(std::unique_ptr<sf::Shape> shape = nullptr)
        : shape(std::move(shape)) {
    }

    explicit RenderableShape(sf::Shape* shape)
        : shape(std::unique_ptr<sf::Shape>(shape)) {
    }

    std::unique_ptr<sf::Shape> shape;
};
