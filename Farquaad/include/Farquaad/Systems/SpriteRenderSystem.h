// Copyright 2016 Bablawn3d5

#pragma once

#include <entityx/entityx.h>
#include <SFML/Graphics.hpp>
#include <Thor/Resources.hpp>

namespace ex = entityx;

struct SpriteRenderSystem : public ex::System<SpriteRenderSystem> {
public:
    explicit SpriteRenderSystem(sf::RenderTarget &targ) : target(targ) {}

    void update(ex::EntityManager &em, ex::EventManager &events, ex::TimeDelta dt);  // NOLINT

private:
    sf::RenderTarget& target;
    thor::ResourceHolder<sf::Font, std::string> font_holder;
    thor::ResourceHolder<sf::Texture, std::string> texture_holder;
};
