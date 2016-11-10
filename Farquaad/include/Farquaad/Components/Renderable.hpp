// Copyright 2016 Bablawn3d5

#pragma once

#include <entityx/entityx.h>
#include <SFML/Graphics.hpp>
#include <string>
#include <map>
#include <vector>

struct Frame {
  std::vector<sf::IntRect> frame_offsets;
  std::vector<entityx::TimeDelta> frame_delay;
  // TODO(SMA) : Currently unused, but will be used as a psudo iterator for
  // frame_offsets.
  unsigned int loop_type = 0;
};

typedef std::map<std::string, Frame> AnimationMap;

// TODO(SMA) : Split this up into multiple components?
struct Renderable {
  // Flag to signify we should rebuild the sprite.
  bool isDirty = true;

  // Animation Stuff
  std::string texture_name;
  std::string current_animation;
  // Current frame can be negative to indicate a offset from the last
  // frame.
  int cur_frame = 0;
  // Current frame time in seconds.
  // HACK(SMA): store as float instead of a duration object, makes it easier to convert
  // from JSON
  float cur_frame_time;
  AnimationMap animations;

  // Font stuff
  std::string font_name;
  std::string font_string;
  uint32_t font_size = 30;
  // HACK(SMA):This doesn't work in metaregister for whatever reason.
  //uint8_t r = 200, g = 0,b = 200, a = 255;
  uint32_t r = 200, g = 0, b = 200, a = 255;
  sf::Vector2f scale{ 1.f,1.f };
  sf::Vector2f offset{ 0.f,0.f };
  float rotation = 0.f;
  uint32_t layer = 0u;

  std::shared_ptr<sf::Drawable> drawable;
  sf::Transformable* transform = nullptr;
  sf::Sprite* sprite = nullptr;
};

struct TextureWithOffsets {
  sf::Image image;
  std::vector<sf::IntRect> offsets;
  AnimationMap anims;
};