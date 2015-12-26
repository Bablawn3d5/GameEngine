// Copyright 2015 Bablawn3d5

#pragma once

#include <Farquaad/Core/SeralizeableComponentMap.h>
#include <Farquaad/Components.hpp>
#include <json/json.h>

template<>
class SerializableHandle<sf::Vector2f> {
public:
    static const RegisteredSerializableComponent<sf::Vector2f> rootName;

    // To be overitten by template specialziations
    inline sf::Vector2f fromJSON(const Json::Value&) const;
    inline Json::Value toJSON(const sf::Vector2f& component) const;
};

template<>
class SerializableHandle<Body> {
public:
    static const RegisteredSerializableComponent<Body> rootName;

    // To be overitten by template specialziations
    inline Body fromJSON(const Json::Value&) const;
    inline Json::Value toJSON(const Body& component) const;
};

#include <Farquaad/Core/JSONSerializedComponents.hpp>
