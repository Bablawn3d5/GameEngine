// Copyright 2015 Bablawn3d5

#pragma once

#include <Farquaad/Core/SeralizeableComponentMap.h>
#include <Farquaad/Core/JSONSerializedPrimitiveTypes.hpp>
#include <Farquaad/Components.hpp>
#include <json/json.h>
#include <map>
#include <string>

template<typename T>
class SerializableHandle<sf::Vector2<T>> {
public:
    inline sf::Vector2<T> fromJSON(const Json::Value&) const;
    inline Json::Value toJSON(const sf::Vector2<T>& component) const;
};

template<>
class SerializableHandle<Body> : public SerializeFromRegistry<Body> {
public:
    static const RegisteredSerializableComponent<Body> rootName;
    SerializableHandle() : SerializeFromRegistry<Body>(this->GenerateMap()) {}
    const SerializeFromRegistry<Body>::MemberMap GenerateMap();
};

template<>
class SerializableHandle<Stats> : public SerializeFromRegistry<Stats> {
public:
    static const RegisteredSerializableComponent<Stats> rootName;
    SerializableHandle() : SerializeFromRegistry<Stats>(this->GenerateMap()) {}
    const SerializeFromRegistry<Stats>::MemberMap GenerateMap();
};

#include <Farquaad/Core/JSONSerializedComponents.hpp> //NOLINT
