// Copyright 2015 Bablawn3d5

#include <Farquaad/Core/JSONSerializedComponents.h>
#include <Farquaad/Core/Serializable.hpp>

const RegisteredSerializableComponent<sf::Vector2f> SerializableHandle<sf::Vector2f>::rootName{ "vector2f" };

inline sf::Vector2f SerializableHandle<sf::Vector2f>::fromJSON(const Json::Value &v) const {
    return sf::Vector2f(v[0].asFloat(), v[1].asFloat());
}

inline Json::Value SerializableHandle<sf::Vector2f>::toJSON(const sf::Vector2f & component) const {
    Json::Value v = Json::arrayValue;
    v[0] = component.x;
    v[1] = component.y;
    return v;
}

const RegisteredSerializableComponent<Body> SerializableHandle<Body>::rootName{ "body" };

inline Body SerializableHandle<Body>::fromJSON(const Json::Value &v) const {
    Body b;
    b.direction = Serializable::fromJSON<sf::Vector2f>(v["direction"]);
    b.position = Serializable::fromJSON<sf::Vector2f>(v["position"]);
    return b;
}

inline Json::Value SerializableHandle<Body>::toJSON(const Body & component) const {
    Json::Value v;
    v["direction"] = Serializable::toJSON(component.direction);
    v["position"] = Serializable::toJSON(component.position);
    return v;
}
