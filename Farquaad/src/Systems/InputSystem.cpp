// Copyright 2017 Bablawn3d5

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

    // Regardless of the current event on frame, send true if the button is being held.
    if ( (k.eventType == sf::Event::MouseButtonPressed || k.eventType == sf::Event::KeyPressed ) &&
         (sf::Mouse::isButtonPressed(k.mouseButton) || sf::Keyboard::isKeyPressed(k.keyCode)) ) {
      return true;
    }

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
    std::set<std::string> triggedEvents;
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
          triggedEvents.insert(pairs.first);
        }
      }
    }

    // Copy events in reverse to list
    this->triggedEvents.clear();
    std::copy(triggedEvents.rbegin(), triggedEvents.rend(), std::back_inserter(this->triggedEvents));

    // Store responders in a refrence vec, and clear old responses
    em.each<InputResponder>(
      [&](ex::Entity entity, InputResponder &responder) {
      auto& responds = responder.responds;
      responds.clear();
      // Copy events to trigger to all responders.
      // HACK(SMA) : Copy in reverse so "+" events get put at the end of the list.
      // This should really be a priority queue.
      std::copy(triggedEvents.rbegin(), triggedEvents.rend(), std::back_inserter(responds));
      responder.mousePos = sf::Mouse::getPosition(window);
      // TODO(SMA) : Translate game window pos to in-game pos.
      // Assming 1:1 here.
      responder.mouseGamePos = sf::Mouse::getPosition(window);
    });

}
