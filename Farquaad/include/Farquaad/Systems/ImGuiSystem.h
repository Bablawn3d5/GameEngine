// Copyright 2016 Bablawn3d5

#pragma once

#include <entityx/entityx.h>
#include <SFML/Graphics.hpp>
#include <SFML/Window/Event.hpp>

#include "imgui.h"
#include "imgui-SFML.h"

namespace ex = entityx;

struct ImGuiSystem : public ex::System<ImGuiSystem> {
public:
    explicit ImGuiSystem(sf::RenderWindow &targ) : targ(targ) {
        ImGui::SFML::Init(targ);
    }

    void update(ex::EntityManager &em, ex::EventManager &events, ex::TimeDelta dt);  // NOLINT
private:
  sf::RenderWindow &targ;
};
