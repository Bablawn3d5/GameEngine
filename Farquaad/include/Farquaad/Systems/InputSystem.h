// Copyright 2015 Bablawn3d5

#pragma once

#include <entityx/entityx.h>
#include <SFML/Window.hpp>
#include <map>
#include <string>

namespace ex = entityx;

enum UserInputType {
    KeyboardInput,
    MouseInput,
    JoystickInput
};

struct KeyInput {
    UserInputType userInputType;
    sf::Event::EventType eventType;
    // Default these to something so they don't wildly get set.
    sf::Keyboard::Key keyCode = sf::Keyboard::Unknown;
    sf::Mouse::Button mouseButton = sf::Mouse::ButtonCount;
};

struct InputSystem : public ex::System<InputSystem> {
public:
    typedef std::map<const std::string, KeyInput> KeyBindMap;

    explicit InputSystem(sf::Window &target) : window(target) {}

    void setKeybinds(const KeyBindMap &);
    void bindEventToKeyPress(const std::string string, const sf::Keyboard::Key key);
    void bindEventToKeyPress(const std::string string, const sf::Mouse::Button button);

    bool testEvent(const std::string eventName, sf::Event e);

    void update(ex::EntityManager &em, ex::EventManager &events, ex::TimeDelta dt);  // NOLINT

    sf::Event events;
private:
    KeyBindMap keyBinds;
    sf::Window& window;
};
