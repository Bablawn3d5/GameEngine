// Copyright 2015 Bablawn3d5

#include <Farquaad/Core/JSONSerializedComponents.h>
#include <Farquaad/Core/Serializable.hpp>

template<typename T>
inline sf::Vector2<T> SerializableHandle<sf::Vector2<T>>::fromJSON(const Json::Value &v) const {
    return sf::Vector2<T>(Serializable::fromJSON<T>(v[0]),
                          Serializable::fromJSON<T>(v[1]));
}

template<typename T>
inline Json::Value SerializableHandle<sf::Vector2<T>>::toJSON(const sf::Vector2<T> & component) const {
    Json::Value v = Json::arrayValue;
    v[0] = component.x;
    v[1] = component.y;
    return v;
}

const RegisteredSerializableComponent<Body> SerializableHandle<Body>::rootName{ "body" };

inline const SerializeFromRegistry<Body>::MemberMap SerializableHandle<Body>::GenerateMap() {
    SerializeFromRegistry<Body>::MemberMap map;
    AddMember(map, "direction", &Body::direction);
    AddMember(map, "position", &Body::position);
    return map;
}

const RegisteredSerializableComponent<Stats> SerializableHandle<Stats>::rootName{ "stats" };

inline const SerializeFromRegistry<Stats>::MemberMap SerializableHandle<Stats>::GenerateMap() {
    SerializeFromRegistry<Stats>::MemberMap map;
    AddMember(map, "curHP", &Stats::currentHp);
    AddMember(map, "maxHP", &Stats::maxHp);
    AddMember(map, "speed", &Stats::speed);
    AddMember(map, "god", &Stats::godmode);
    return map;
}
