// Copyright 2015 Bablawn3d5
#define CATCH_CONFIG_MAIN

#include <Farquaad/Serialization.hpp>
#include <SFML/System.hpp>
#include <entityx/entityx.h>
#include <string>
#include <sstream>
#include <iostream>
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

template<typename T>
std::ostream &operator<<(std::ostream &out, const sf::Vector2<T> &v) {
    out << "Vector2(" << v.x << ", " << v.y << ")";
    return out;
}

bool operator==(const Stats &r, const Stats &l) {
    return r.currentHp == l.currentHp &&
        r.maxHp == l.maxHp &&
        r.speed == l.speed &&
        r.godmode == l.godmode;
}

std::ostream &operator<<(std::ostream &out, const Stats &s) {
    out << "Stats(" << s.currentHp << "/" << s.maxHp << ","
        << s.speed << "," << s.godmode << "," << ")";
    return out;
}

std::ostream &operator<<(std::ostream &out, const Body &b) {
    out << "Body(" << b.direction << ", " << b.position << ")";
    return out;
}

std::string toString(const Json::Value &value) {
    Json::FastWriter w;
    return w.write(value);
}

// TODO(SMA) : Do we need this?
struct JSONSerializedFixture {
};

TEST_CASE_METHOD(JSONSerializedFixture, "TestComponentStats") {
    Stats s{ 100.999f, 200000.0f, -20000.0f };
    s.godmode = true;
    Json::Value v = Serializable::toJSON(s);
    REQUIRE(v.isObject());
    REQUIRE(size(v) == 4);
    REQUIRE(v["curHP"].asFloat() == s.currentHp);
    REQUIRE(v["maxHP"].asFloat() == s.maxHp);
    REQUIRE(v["speed"].asFloat() == s.speed);
    REQUIRE(v["god"].asBool() == s.godmode);

    Stats actual = Serializable::fromJSON<Stats>(v);
    REQUIRE(v["curHP"].asFloat() == actual.currentHp);
    REQUIRE(v["maxHP"].asFloat() == actual.maxHp);
    REQUIRE(v["speed"].asFloat() == actual.speed);
    REQUIRE(v["god"].asBool() == actual.godmode);
    REQUIRE(s == actual);
}

TEST_CASE_METHOD(JSONSerializedFixture, "TestComponentBody") {
    Body b{ 10.0f, 19.99f, -20.0f, 30.2451f };
    Json::Value v = Serializable::toJSON(b);
    REQUIRE(v.isObject());
    REQUIRE(size(v) == 2);
    REQUIRE(v["direction"][0].asFloat() == b.direction.x);
    REQUIRE(v["direction"][1].asFloat() == b.direction.y);
    REQUIRE(v["position"][0].asFloat() == b.position.x);
    REQUIRE(v["position"][1].asFloat() == b.position.y);

    Body actual = Serializable::fromJSON<Body>(v);
    REQUIRE(v["direction"][0].asFloat() == actual.direction.x);
    REQUIRE(v["direction"][1].asFloat() == actual.direction.y);
    REQUIRE(v["position"][0].asFloat() == actual.position.x);
    REQUIRE(v["position"][1].asFloat() == actual.position.y);
    REQUIRE(actual.direction == b.direction);
    REQUIRE(actual.position == b.position);
}

TEST_CASE_METHOD(JSONSerializedFixture, "TestVector2f") {
    sf::Vector2f b{ 10.2f, -1.999998f };
    Json::Value v = Serializable::toJSON(b);
    REQUIRE(v.isArray());
    REQUIRE(size(v) == 2);
    REQUIRE(v[0].asFloat() == b.x);
    REQUIRE(v[1].asFloat() == b.y);

    sf::Vector2f actual = Serializable::fromJSON<sf::Vector2f>(v);
    REQUIRE(v[0].asFloat() == actual.x);
    REQUIRE(v[1].asFloat() == actual.y);
    REQUIRE(b == actual);
}

TEST_CASE_METHOD(JSONSerializedFixture, "TestBodyFromString") {
    Json::Value v;
    std::stringstream s;
    SECTION("Has both position and direction") {
        s << "{\"direction\":[-0.00012,99.123],\"position\":[10,50000000]}";
        s >> v;
        REQUIRE(v.isObject());
        REQUIRE(size(v) == 2);

        Body actual = Serializable::fromJSON<Body>(v);
        Body expected(10.0f, 50000000.0f, -0.00012f, 99.123f);
        REQUIRE(v["direction"][0].asFloat() == actual.direction.x);
        REQUIRE(v["direction"][1].asFloat() == actual.direction.y);
        REQUIRE(v["position"][0].asFloat() == actual.position.x);
        REQUIRE(v["position"][1].asFloat() == actual.position.y);
        REQUIRE(v["direction"][0].asFloat() == expected.direction.x);
        REQUIRE(v["direction"][1].asFloat() == expected.direction.y);
        REQUIRE(v["position"][0].asFloat() == expected.position.x);
        REQUIRE(v["position"][1].asFloat() == expected.position.y);
        REQUIRE(actual.direction == expected.direction);
        REQUIRE(actual.position == expected.position);
    }

    SECTION("Has only direction") {
        s << "{\"direction\":[-0.00012,-5999.1234]}";
        s >> v;
        REQUIRE(v.isObject());
        REQUIRE(size(v) == 1);

        Body actual = Serializable::fromJSON<Body>(v);
        Body expected;
        expected.direction = { -0.00012f, -5999.1234f };
        REQUIRE(v["direction"][0].asFloat() == actual.direction.x);
        REQUIRE(v["direction"][1].asFloat() == actual.direction.y);
        REQUIRE(v["position"][0].asFloat() == actual.position.x);
        REQUIRE(v["position"][1].asFloat() == actual.position.y);
        REQUIRE(v["direction"][0].asFloat() == expected.direction.x);
        REQUIRE(v["direction"][1].asFloat() == expected.direction.y);
        REQUIRE(v["position"][0].asFloat() == expected.position.x);
        REQUIRE(v["position"][1].asFloat() == expected.position.y);
        REQUIRE(actual.direction == expected.direction);
        REQUIRE(actual.position == expected.position);
    }
    SECTION("Has only position") {
        s << "{\"position\":[-0.00012,-5999.1234]}";
        s >> v;
        REQUIRE(v.isObject());
        REQUIRE(size(v) == 1);

        Body actual = Serializable::fromJSON<Body>(v);
        Body expected;
        expected.position = { -0.00012f, -5999.1234f };
        REQUIRE(v["direction"][0].asFloat() == actual.direction.x);
        REQUIRE(v["direction"][1].asFloat() == actual.direction.y);
        REQUIRE(v["position"][0].asFloat() == actual.position.x);
        REQUIRE(v["position"][1].asFloat() == actual.position.y);
        REQUIRE(v["direction"][0].asFloat() == expected.direction.x);
        REQUIRE(v["direction"][1].asFloat() == expected.direction.y);
        REQUIRE(v["position"][0].asFloat() == expected.position.x);
        REQUIRE(v["position"][1].asFloat() == expected.position.y);
        REQUIRE(actual.direction == expected.direction);
        REQUIRE(actual.position == expected.position);
    }
}

TEST_CASE_METHOD(JSONSerializedFixture, "TestVector2fFromString") {
    Json::Value v;
    std::stringstream s;
    s << "[ -0.000012, 99.12345 ]";
    s >> v;

    REQUIRE(v.isArray());
    REQUIRE(size(v) == 2);
    sf::Vector2f actual = Serializable::fromJSON<sf::Vector2f>(v);
    sf::Vector2f expected(-0.000012f, 99.12345f);
    REQUIRE(v[0].asFloat() == actual.x);
    REQUIRE(v[1].asFloat() == actual.y);
    REQUIRE(expected == actual);
}
