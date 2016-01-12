// Copyright 2015-2016 Bablawn3d5
#include <Farquaad/Serialization.hpp>
#include <Farquaad/Core/JSONSerializedSystemObjects.hpp>
#include <Farquaad/Systems.hpp>
#include <json/json.h>
#include <string>
#include <sstream>
#include <iostream>
#include <catch.hpp>

template <typename T>
int csize(const T &t) {
    int n = 0;
    for ( auto i : t ) {
        ++n;
        (void)i;  // Unused on purpose, suppress warning
    }
    return n;
}

std::ostream &operator<<(std::ostream &out, const KeyInput &v) {
    out << "KeyInput(" << v.userInputType << ", " <<
        v.keyCode << ", " <<
        v.mouseButton << ", " <<
        v.eventType << ")";
    return out;
}

bool operator==(const KeyInput& a, const KeyInput& b) {
    return a.userInputType == b.userInputType &&
        a.keyCode == b.keyCode &&
        a.mouseButton == b.mouseButton &&
        a.eventType == b.eventType;
}

bool operator!=(const KeyInput& a, const KeyInput& b) {
    return !(a == b);
}

std::ostream &operator<<(std::ostream &out, const InputSystem::KeyBindMap& a) {
    for ( auto& pair : a ) {
        std::string s = pair.first;
        out << "Key( " << s << " ) : " << pair.second << std::endl;
    }
    return out;
}

bool operator==(const InputSystem::KeyBindMap& a, const InputSystem::KeyBindMap& b) {
    for ( auto& pair : a ) {
        std::string s = pair.first;
        if ( b.at(s) != a.at(s) )
            return 0;
    }

    for ( auto& pair : b ) {
        std::string s = pair.first;
        if ( b.at(s) != a.at(s) )
            return 0;
    }

    return 1;
}

std::string toString(const Json::Value &value) {
    Json::FastWriter w;
    return w.write(value);
}

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
