// Copyright 2016 Bablawn3d5

#pragma once

#include <SFML/Audio.hpp>
#include <string>

struct Sound {
    std::string name;
    float pitch = 1.0f;
    float volume = 100.0f;
    float start_delay = 0.f;
    int play_count = 1;

    std::shared_ptr<sf::Sound> sound;
};
