// Copyright 2015 Bablawn3d5

#include <Farquaad/Systems/InputSystem.h>
#include <string>

void InputSystem::bindEventToKeyPress(const std::string string, sf::Keyboard::Key key) {
    KeyInput bind;
    bind.EventType = sf::Event::KeyPressed;
    bind.KeyCode = key;
    keyBinds[string] = bind;

    return;
}

void InputSystem::bindEventToKeyPress(const std::string string, sf::Mouse::Button button) {
    KeyInput bind;
    bind.EventType = sf::Event::MouseButtonPressed;
    bind.MouseButton = button;
    keyBinds[string] = bind;

    return;
}

bool InputSystem::testEvent(const std::string eventName, sf::Event e) {
    try {
        (void)keyBinds.at(eventName);
    }
    catch ( std::out_of_range e ) {
        std::cerr << "Keybind for event: '" << eventName << "' does not exist!" << std::endl;
    }

    KeyInput& k = keyBinds[eventName];

    // Mouse event
    if ( k.EventType == e.type &&
        k.MouseButton == e.mouseButton.button ) {
        return (true);
    }

    // Keyboard event
    if ( k.EventType == e.type &&
        k.KeyCode == e.key.code ) {
        return (true);
    }
    return (false);
}

void InputSystem::update(ex::EntityManager &em,
                         ex::EventManager &events, ex::TimeDelta dt) {
    sf::Event Event;

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
        if ( testEvent("Move", Event) ) {
            std::cout << "Move !" << std::endl;
        }
        if ( testEvent("Error Move!", Event) ) {
        }
        if ( testEvent("Use", Event) ) {
            std::cout << "Use !" << std::endl;
        }
    }
}
