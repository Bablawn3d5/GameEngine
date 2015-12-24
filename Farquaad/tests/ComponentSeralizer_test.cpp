// Copyright 2015 Bablawn3d5
#define CATCH_CONFIG_MAIN

#include <Farquaad/Core.hpp>
#include <string>
#include <sstream>
// TODO(SMA): Fix these includes...
#include "entityx/3rdparty/catch.hpp"

template <typename T>
int size(const T &t) {
    int n = 0;
    for ( auto i : t ) {
        ++n;
        (void)i;  // Unused on purpose, suppress warning
    }
    return n;
}

struct Position {
    explicit Position(float x = 0.0f, float y = 0.0f) : x(x), y(y) {}

    bool operator==(const Position &other) const {
        return x == other.x && y == other.y;
    }

    float x, y;
};

template<>
struct SerializableHandle<Position> {
    static const std::string rootName;

    Position fromJSON(const Json::Value& json) {
        Position p;
        p.x = json["x"].asFloat();
        p.y = json["y"].asFloat();
        return p;
    }

    Json::Value toJSON(const Position& component) {
        Json::Value v;
        v["x"] = component.x;
        v["y"] = component.y;
        return v;
    }
};

const std::string SerializableHandle<Position>::rootName = "pos";

std::ostream &operator<<(std::ostream &out, const Position &position) {
    out << "Position(" << position.x << ", " << position.y << ")";
    return out;
}

std::string toString(const Json::Value &value) {
    std::stringstream s;
    s << value;
    return s.str();
}

struct ComponentSeralizerTestFixture {};

TEST_CASE_METHOD(ComponentSeralizerTestFixture, "TestDefaultConstructor") {
    ComponentSerializer cs;
    REQUIRE(cs.toString() == "null");
}

TEST_CASE_METHOD(ComponentSeralizerTestFixture, "TestJSONConstructor") {
    Json::Value root;
    ComponentSerializer cs0(root);
    REQUIRE(cs0.toString() == "null");

    Json::Value root2;
    root2["foo"] = "bar";
    Json::Value expected = root2;
    ComponentSerializer cs1(root2);
    REQUIRE(cs1.toString() == toString(expected));

    // Is copy.
    root2["bar"] = 2;
    REQUIRE(cs1.toString() == toString(expected));
}

TEST_CASE_METHOD(ComponentSeralizerTestFixture, "TestParseEntityString") {
    ComponentSerializer cs0;
    REQUIRE(cs0.toString() == "null");

    int ret = cs0.ParseEntityString("{\"foo\" : \"bar\"}");
    REQUIRE(ret == 1);
    Json::Value root;
    root["foo"] = "bar";
    REQUIRE(cs0.toString() == toString(root));
}

TEST_CASE_METHOD(ComponentSeralizerTestFixture, "TestLoadComponent") {
    ComponentSerializer cs0;
    REQUIRE(cs0.toString() == "null");

    // Empty
    Position p(1.0f, 2.0f);
    Position expected(0.0f, 0.0f);
    cs0.Load<Position>(p);
    REQUIRE(p == expected);

    // With Values
    Position expected1(200.0f, 210.0f);
    Position p1;
    Json::Value v;
    v[SerializableHandle<Position>::rootName] = Serializable<Position>::toJSON(expected1);
    ComponentSerializer cs1(v);
    cs1.Load<Position>(p1);
    REQUIRE(p1 == expected1);
}

TEST_CASE_METHOD(ComponentSeralizerTestFixture, "TestSaveComponent") {
    Position expected1(200.0f, 100.0f);
    Json::Value v;
    v[SerializableHandle<Position>::rootName] = Serializable<Position>::toJSON(expected1);
    ComponentSerializer cs0;
    REQUIRE(cs0.toString() == "null");
    std::string actual = cs0.Save(expected1);
    REQUIRE(actual == toString(v["pos"]));
}
