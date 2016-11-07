// Copyright 2016 Bablawn3d5

#pragma once

#include <entityx/entityx.h>
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include <Farquaad/Core/LevelResourceLoader.h>

namespace ex = entityx;

struct SpriteRenderSystem : public ex::System<SpriteRenderSystem> {
public:
    // Maximum number of rendering layers user can use.
    const size_t max_layers;
    explicit SpriteRenderSystem(sf::RenderTarget &targ, 
                                LevelResoruceLoader& resource,
                                const size_t& l = 10u) : max_layers(l), target(targ), holder(resource) {}

    void update(ex::EntityManager &em, ex::EventManager &events, ex::TimeDelta dt);  // NOLINT

private:
    sf::RenderTarget& target;
    LevelResoruceLoader& holder;
    thor::ResourceHolder<sf::Texture, std::string> texture_holder;
};
