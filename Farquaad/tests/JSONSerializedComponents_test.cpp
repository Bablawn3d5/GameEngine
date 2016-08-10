// Copyright 2015-2016 Bablawn3d5
#include <Farquaad/Serialization.hpp>
#include <Common.h>
#include <SFML/System.hpp>
#include <entityx/entityx.h>
#include <string>
#include <sstream>
#include <iostream>
#include <catch.hpp>

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

TEST_CASE_METHOD(JSONSerializedFixture, "TestInputResponder") {
    InputResponder responder;
    Json::Value v = Serializable::toJSON(responder);
    REQUIRE(v.isObject());
}

TEST_CASE_METHOD(JSONSerializedFixture, "TestPhysics") {
    Physics p;
    SECTION("Physics Size") {
        Json::Value v = Serializable::toJSON(p);
        REQUIRE(v.isObject());
        REQUIRE(size(v) == 11);
        REQUIRE(v["bodyType"].asString() == "STATIC");
        REQUIRE(v["size"][0].asFloat() == p.size.x);
        REQUIRE(v["size"][1].asFloat() == p.size.y);
        REQUIRE(v["dirty"].asBool() == true);
        REQUIRE(v["category"].asString() == "CATEGORY_1");
        REQUIRE(v["mask"].asString() == "0000000000000001");
        REQUIRE(v["group"].asInt() == 0);


        Physics actual = Serializable::fromJSON<Physics>(v);
        REQUIRE(actual.bodyType == p.bodyType);
        REQUIRE(actual.size.x == p.size.x);
        REQUIRE(actual.size.y == p.size.y);
        REQUIRE(actual.isDirty == p.isDirty);
        REQUIRE(actual.collisionCategory == p.collisionCategory);
        REQUIRE(actual.collisionMask == p.collisionMask);
        REQUIRE(actual.collisionGroup == p.collisionGroup);
    }
    SECTION("Static Body") {
        Json::Value v = Serializable::toJSON(p);
        REQUIRE(v.isObject());
        REQUIRE(size(v) == 11);
        REQUIRE(v["bodyType"].asString() == "STATIC");

        Physics actual = Serializable::fromJSON<Physics>(v);
        REQUIRE(actual.bodyType == p.bodyType);
    }

    SECTION("Dynamic Body") {
        p.bodyType = b2_dynamicBody;
        Json::Value v = Serializable::toJSON(p);
        REQUIRE(v.isObject());
        REQUIRE(size(v) == 11);
        REQUIRE(v["bodyType"].asString() == "DYNAMIC");

        Physics actual = Serializable::fromJSON<Physics>(v);
        REQUIRE(actual.bodyType == p.bodyType);
    }

    SECTION("Kinematic Body") {
        p.bodyType = b2_kinematicBody;
        Json::Value v = Serializable::toJSON(p);
        REQUIRE(v.isObject());
        REQUIRE(size(v) == 11);
        REQUIRE(v["bodyType"].asString() == "KINEMATIC");

        Physics actual = Serializable::fromJSON<Physics>(v);
        REQUIRE(actual.bodyType == p.bodyType);
    }
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
