// Copyright 2015 Bablawn3d5

#include <Farquaad/Systems/InputSystem.h>
#include <Farquaad/Components.hpp>
#include <string>

void InputSystem::setKeybinds(const KeyBindMap & binds) {
    this->keyBinds = binds;
}

void InputSystem::bindEventToKeyPress(const std::string string, sf::Keyboard::Key key) {
    KeyInput bind;
    bind.userInputType = KeyboardInput;
    bind.eventType = sf::Event::KeyPressed;
    bind.keyCode = key;
    keyBinds["+" + string] = bind;

    bind.eventType = sf::Event::KeyReleased;
    bind.keyCode = key;
    keyBinds["-" + string] = bind;

    return;
}

void InputSystem::bindEventToKeyPress(const std::string string, sf::Mouse::Button button) {
    KeyInput bind;
    bind.userInputType = MouseInput;
    bind.eventType = sf::Event::MouseButtonPressed;
    bind.mouseButton = button;
    keyBinds["+" + string] = bind;

    bind.eventType = sf::Event::MouseButtonReleased;
    bind.mouseButton = button;
    keyBinds["-" + string] = bind;

    return;
}

bool InputSystem::testEvent(const std::string eventName, sf::Event e) {
    try {
        (void)keyBinds.at(eventName);
    }
    catch ( std::out_of_range e ) {
        std::cerr << "[Error] Keybind for event: '" << eventName << "' does not exist!" << std::endl;
        std::cerr << "[Warning] A placeholder unknown keybind will be created." << std::endl;
    }

    KeyInput& k = keyBinds[eventName];

    // Mouse event
    if ( k.eventType == e.type &&
        k.mouseButton == e.mouseButton.button ) {
        return (true);
    }

    // Keyboard event
    if ( k.eventType == e.type &&
        k.keyCode == e.key.code ) {
        return (true);
    }
    return (false);
}

void InputSystem::update(ex::EntityManager &em,
                         ex::EventManager &events, ex::TimeDelta dt) {
    sf::Event Event;

    sf::Vector2f direction;
    bool processedEvents = false;
    // Poll window Events
    while ( window.pollEvent(Event) ) {
        // Specail Case: Window closed
        // TODO(SMA) : Handle window close.
        if ( Event.type == sf::Event::Closed ) {
            window.close();
        }

        // TODO(SMA): Iterate though entities that subscribe to events
        // and do stuff when a event is triggered.
        // HACK(SMA): For now, register some static events.
        if ( testEvent("+MoveUp", Event) ) {
            direction.y -= 1;
        }
        if ( testEvent("+MoveDown", Event) ) {
            direction.y += 1;
        }
        if ( testEvent("+MoveLeft", Event) ) {
            direction.x -= 1;
        }
        if ( testEvent("+MoveRight", Event) ) {
            direction.x += 1;
        }

        if ( testEvent("-MoveUp", Event) ) {
            direction.y += 1;
        }
        if ( testEvent("-MoveDown", Event) ) {
            direction.y -= 1;
        }
        if ( testEvent("-MoveLeft", Event) ) {
            direction.x += 1;
        }
        if ( testEvent("-MoveRight", Event) ) {
            direction.x -= 1;
        }

        if ( testEvent("+Use", Event) ) {
            std::cout << "Use !" << std::endl;
        }
        processedEvents = true;
    }

    if ( processedEvents == false )
        return;

    // Process direction.
    em.each<Body, InputResponder>(
        [this, &direction](ex::Entity, Body& body, InputResponder& responder) {
        body.direction += direction;
    });
}
