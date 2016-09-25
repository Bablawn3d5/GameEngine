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

struct Renderable {
    std::string texture_name;
    std::string sprite_name;

    std::string current_animation;
    size_t cur_frame = 0;
    entityx::TimeDelta cur_frame_time = 0;
    std::map<std::string, Frame> animations;
    std::shared_ptr<sf::Drawable> drawable;
    std::weak_ptr<sf::Transformable> transform;
    std::weak_ptr<sf::Sprite> sprite;
};
