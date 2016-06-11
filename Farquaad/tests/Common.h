// Copyright 2016 Bablawn3d5

#pragma once

#include <Farquaad/Serialization.hpp>
#include <Farquaad/Core.hpp>
#include <Farquaad/Core/ComponentSerializer.h>
#include <Farquaad/Components.hpp>
#include <Farquaad/Systems/InputSystem.h>
#include <entityx/entityx.h>
#include <json/json.h>
#include <string>
#include <Meta.h>

template <typename T>
int size(const T &t) {
    int n = 0;
    for ( auto i : t ) {
        ++n;
        (void)i;  // Unused on purpose, suppress warning
    }
    return n;
}

// For silly containers.
template <typename T>
int csize(const T &t) {
    int n = 0;
    for ( auto i : t ) {
        ++n;
        (void)i;  // Unused on purpose, suppress warning
    }
    return n;
}

bool operator==(const Body &r, const Body &l);
bool operator==(const Stats &r, const Stats &l);
bool operator==(const KeyInput& a, const KeyInput& b);
bool operator!=(const KeyInput& a, const KeyInput& b);
bool operator==(const InputSystem::KeyBindMap& a, const InputSystem::KeyBindMap& b);

std::string toString(const Json::Value &value);
std::string toStyledString(const Json::Value &value);

struct Direction {
    explicit Direction(float x = 0.0, float y = 0.0) : x(x), y(y) {}

    float x, y;
};

struct Position {
    explicit Position(float x = 0.0f, float y = 0.0f) : x(x), y(y) {}

    bool operator==(const Position &other) const {
        return x == other.x && y == other.y;
    }

    float x, y;
};

namespace meta {
template <>
constexpr auto registerName<Position>() {
  return "pos";
}

template <>
inline auto registerMembers<Position>() {
  return members(
    member("x", &Position::x),
    member("y", &Position::y)
    );
}
} // namespace meta

template<>
class SerializableHandle<Position>  {
public:
    SerializableHandle() {}

    Position fromJSON(const Json::Value& json) const {
        Position p;
        p.x = json["x"].asFloat();
        p.y = json["y"].asFloat();
        return p;
    }

    Json::Value toJSON(const Position& component) const {
        Json::Value v;
        v["x"] = component.x;
        v["y"] = component.y;
        return v;
    }
};

std::ostream &operator<<(std::ostream &out, const Position &position);

template<typename T>
std::ostream &operator<<(std::ostream &out, const sf::Vector2<T> &v) {
    out << "Vector2(" << v.x << ", " << v.y << ")";
    return out;
}

std::ostream &operator<<(std::ostream &out, const Stats &s);
std::ostream &operator<<(std::ostream &out, const Body &b);
std::ostream &operator<<(std::ostream &out, const KeyInput &v);
std::ostream &operator<<(std::ostream &out, const InputSystem::KeyBindMap& a);
