// Copyright 2016 Bablawn3d5

#pragma once

#include <entityx/entityx.h>
#include <SFML/Audio.hpp>
#include <Thor/Resources.hpp>
#include <string>

namespace ex = entityx;

struct SoundSystem : public ex::System<SoundSystem> {
public:
  explicit SoundSystem();
  void update(ex::EntityManager &em, ex::EventManager &events, ex::TimeDelta dt);  // NOLINT

  thor::ResourceHolder<sf::SoundBuffer, std::string> soundbuff_holder;
};
