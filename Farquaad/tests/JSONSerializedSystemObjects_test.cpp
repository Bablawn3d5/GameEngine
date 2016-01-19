// Copyright 2015-2016 Bablawn3d5
#include <Farquaad/Serialization.hpp>
#include <Farquaad/Core/JSONSerializedSystemObjects.hpp>
#include <Farquaad/Systems.hpp>
#include <Common.h>
#include <json/json.h>
#include <catch.hpp>
#include <string>
#include <sstream>
#include <iostream>

KeyInput createKeyPressInput(sf::Keyboard::Key key) {
    KeyInput bind;
    bind.userInputType = KeyboardInput;
    bind.keyCode = key;
    return bind;
}

KeyInput createMousePressInput(sf::Mouse::Button button) {
    KeyInput bind;
    bind.userInputType = MouseInput;
    bind.mouseButton = button;
    return bind;
}

// TODO(SMA) : Do we need this?
struct JSONSerializedFixture {
};

TEST_CASE_METHOD(JSONSerializedFixture, "TestInputSystem_KeyBindMap") {
    InputSystem::KeyBindMap map;

    KeyInput bind = createKeyPressInput(sf::Keyboard::A);
    bind.eventType = sf::Event::KeyPressed;
    map["+EventA"] = bind;
    bind.eventType = sf::Event::KeyReleased;
    map["-EventA"] = bind;
    REQUIRE(csize(map) == 2);
    Json::Value v;
    v = Serializable::toJSON(map);
    REQUIRE(v["EventA"] == "KEY_A");
    REQUIRE(csize(v) == 1);

    InputSystem::KeyBindMap real = Serializable::fromJSON<InputSystem::KeyBindMap>(v);
    REQUIRE(map == real);
    REQUIRE(csize(real) == 2);
}

TEST_CASE_METHOD(JSONSerializedFixture, "TestInputSystem_MouseBindMap") {
    InputSystem::KeyBindMap map;
    KeyInput bind = createMousePressInput(sf::Mouse::Left);
    bind.eventType = sf::Event::MouseButtonPressed;
    map["+EventB"] = bind;
    bind.eventType = sf::Event::MouseButtonReleased;
    map["-EventB"] = bind;
    REQUIRE(csize(map) == 2);
    Json::Value v;
    v = Serializable::toJSON(map);
    REQUIRE(v["EventB"] == "MOUSE_Left");
    REQUIRE(csize(v) == 1);

    InputSystem::KeyBindMap real = Serializable::fromJSON<InputSystem::KeyBindMap>(v);
    REQUIRE(map == real);
    REQUIRE(csize(real) == 2);
}
