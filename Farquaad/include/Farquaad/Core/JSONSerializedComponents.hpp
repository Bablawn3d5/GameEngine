// Copyright 2015 Bablawn3d5

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
    // Workaround: DR253: http://www.open-std.org/jtc1/sc22/wg21/docs/cwg_active.html#253
    // Define these so class becomes non POD for const initalizaiton
    SerializableHandle() {}
    ~SerializableHandle() {}

    inline sf::Vector2<T> fromJSON(const Json::Value &v) const {
        return sf::Vector2<T>(Serializable::fromJSON<T>(v[0]),
                              Serializable::fromJSON<T>(v[1]));
    }

    inline Json::Value toJSON(const sf::Vector2<T> & component) const {
        Json::Value v = Json::arrayValue;
        v[0] = component.x;
        v[1] = component.y;
        return v;
    }
};

template<>
class SerializableHandle<Body> : public SerializeFromRegistry<Body>,
    public MappedComponent<Body> {
public:
    SerializableHandle() : SerializeFromRegistry<Body>(this->GenerateMap()),
        MappedComponent("body") {
    }

    inline const SerializeFromRegistry<Body>::MemberMap GenerateMap() {
        SerializeFromRegistry<Body>::MemberMap map;
        AddMember(map, "direction", &Body::direction);
        AddMember(map, "position", &Body::position);
        return map;
    }
};

template<>
class SerializableHandle<Stats> : public SerializeFromRegistry<Stats>,
    public MappedComponent<Stats> {
public:
    SerializableHandle() : SerializeFromRegistry<Stats>(this->GenerateMap()),
        MappedComponent("stats") {
    }

    inline const SerializeFromRegistry<Stats>::MemberMap GenerateMap() {
        SerializeFromRegistry<Stats>::MemberMap map;
        AddMember(map, "curHP", &Stats::currentHp);
        AddMember(map, "maxHP", &Stats::maxHp);
        AddMember(map, "speed", &Stats::speed);
        AddMember(map, "god", &Stats::godmode);
        return map;
    }
};
