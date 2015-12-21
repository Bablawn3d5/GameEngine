// Copyright 2015 Babalawn3d5

#pragma once

#include <entityx/entityx.h>
#include <SFML/Window.hpp>
#include <map>
#include <string>

namespace ex = entityx;

struct KeyInput {
    sf::Event::EventType EventType;
    sf::Keyboard::Key KeyCode;
    sf::Mouse::Button MouseButton;
};

struct InputSystem : public ex::System<InputSystem> {
    typedef std::map<const std::string, KeyInput> KeyBindMap;

    KeyBindMap keyBinds;
    sf::Window& window;

    explicit InputSystem(sf::Window &target) : window(target) { };

    void bindEventToKeyPress(const std::string string, const sf::Keyboard::Key key);
    void bindEventToKeyPress(const std::string string, const sf::Mouse::Button button);

    bool testEvent(const std::string eventName, sf::Event e);

    void update(ex::EntityManager &em, ex::EventManager &events, ex::TimeDelta dt);
};
