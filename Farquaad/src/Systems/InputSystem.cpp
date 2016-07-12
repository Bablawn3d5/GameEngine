// Copyright 2015 Bablawn3d5

#include <Farquaad/Systems/InputSystem.h>
#include <Farquaad/Components.hpp>
#include "imgui-SFML.h"
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

    // Store responders in a refrence vec, and clear old responses
    // FIXME(SMA) : This lookup may be expensive when we have tons of entities.
    std::vector<std::reference_wrapper<InputResponder>> responders;
    em.each<InputResponder>(
      [&](ex::Entity entity, InputResponder &responder) {
      responder.responds.clear();
      responders.push_back(responder);
      responder.mousePos = sf::Mouse::getPosition(window);
      // TODO(SMA) : Translate game window pos to in-game pos.
      // Assming 1:1 here.
      responder.mouseGamePos = sf::Mouse::getPosition(window);
    });

    // Poll window Events
    while ( window.pollEvent(Event) ) {
      // HACK(SMA) : Process the event right here.
      ImGui::SFML::ProcessEvent(Event);

      // Specail Case: Window closed
      // TODO(SMA) : Handle other window cases.
      // TODO(SMA) : Brodcast window close
      if ( Event.type == sf::Event::Closed ) {
        window.close();
      }

      // HACK (SMA) : Push event name into all responders
      // This looks cancerous but it works.
      for ( auto& pairs : keyBinds ) {
        if ( testEvent(pairs.first, Event) ) {
          for ( auto& responder : responders ) {
            responder.get().responds.push_back(pairs.first);
          }
        }
      }
    }
}
