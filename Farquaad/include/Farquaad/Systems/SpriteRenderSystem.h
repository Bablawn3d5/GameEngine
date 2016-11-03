// Copyright 2016 Bablawn3d5

#pragma once

#include <entityx/entityx.h>
#include <SFML/Graphics.hpp>
#include <Thor/Resources.hpp>
#include <vector>
#include <string>
namespace ex = entityx;

// From Renderable.hpp animations.
struct Frame;
typedef std::map<std::string, Frame> AnimationMap;

struct TextureWithOffsets {
  sf::Texture tex;
  std::vector<sf::IntRect> offsets;
  AnimationMap anims;
};

struct SpriteRenderSystem : public ex::System<SpriteRenderSystem> {
public:
    // Maximum number of rendering layers user can use.
    const size_t max_layers;
    explicit SpriteRenderSystem(sf::RenderTarget &targ, const size_t& l = 10u) : max_layers(l), target(targ) {}

    void update(ex::EntityManager &em, ex::EventManager &events, ex::TimeDelta dt);  // NOLINT

private:
    sf::RenderTarget& target;
    thor::ResourceHolder<sf::Font, std::string> font_holder;
    thor::ResourceHolder<TextureWithOffsets, std::string> texture_holder;
};
