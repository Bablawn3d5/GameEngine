// Copyright 2015-2016 Bablawn3d5
#include <Farquaad/Serialization.hpp>
#include <Farquaad/Core/SeralizeableComponentMap.h>
#include <Farquaad/Core/ComponentSerializer.h>
#include <Farquaad/Core.hpp>
#include <Common.h>
#include <catch.hpp>
#include <entityx/entityx.h>
#include <string>
#include <sstream>
#include <iostream>

using entityx::Entity;
using entityx::EventManager;
using entityx::EntityManager;

struct ComponentSeralizerTestFixture {
    ComponentSeralizerTestFixture() : em(ev) {}
    EntityManager em;
    EventManager ev;
};

TEST_CASE_METHOD(ComponentSeralizerTestFixture, "ComponentSeralizerTestDefaultConstructor") {
    ComponentSerializer cs;
    REQUIRE(cs.toString() == "null\n");
}

TEST_CASE_METHOD(ComponentSeralizerTestFixture, "TestJSONConstructor") {
    Json::Value root;
    ComponentSerializer cs0(root);
    REQUIRE(cs0.toString() == "null\n");

    Json::Value root2;
    root2["foo"] = "bar";
    Json::Value expected = root2;
    ComponentSerializer cs1(root2);
    REQUIRE(cs1.toString() == toStyledString(expected));

    // Is copy.
    root2["bar"] = 2;
    REQUIRE(cs1.toString() == toStyledString(expected));
}

TEST_CASE_METHOD(ComponentSeralizerTestFixture, "TestParseEntityString") {
    ComponentSerializer cs0;
    REQUIRE(cs0.toString() == "null\n");

    SECTION("Load good string") {
        int ret = cs0.ParseEntityString("{\"foo\" : \"bar\"}");
        REQUIRE(ret == 1);
        Json::Value root;
        root["foo"] = "bar";
        REQUIRE(cs0.toString() == toStyledString(root));
    }
    SECTION("Load bad string") {
        // Redirect cerr to test
        std::ostringstream sstream;
        std::streambuf* errbuf = std::cerr.rdbuf(sstream.rdbuf());
        int ret = cs0.ParseEntityString("{'bad json'}");
        REQUIRE(ret == 0);
        REQUIRE(cs0.toString() == "null\n");

        // Restore std::err
        std::cerr.rdbuf(errbuf);
    }
}

TEST_CASE_METHOD(ComponentSeralizerTestFixture, "TestHasComponent") {
    ComponentSerializer cs0;
    REQUIRE(cs0.toString() == "null\n");
    REQUIRE(cs0.HasComponent<Position>() == false);

    // Load works with values loaded
    Position expected1(250.99999999f, 210.5f);
    Json::Value v;
    v["pos"] = Serializable::toJSON(expected1);
    ComponentSerializer cs1(v);
    REQUIRE(cs1.HasComponent<Position>() == true);
}

TEST_CASE_METHOD(ComponentSeralizerTestFixture, "TestLoadComponent") {
    ComponentSerializer cs0;
    REQUIRE(cs0.toString() == "null\n");

    // Load works from empty
    Position p(1.0f, 2.0f);
    p = cs0.Load<Position>();
    Position expected(0.0f, 0.0f);
    REQUIRE(p == expected);

    // Load works with values loaded
    Position expected1(250.99999999f, 210.5f);
    Position p1;
    Json::Value v;
    v["pos"] = Serializable::toJSON(expected1);
    ComponentSerializer cs1(v);
    p1 = cs1.Load<Position>();
    REQUIRE(p1 == expected1);
}

TEST_CASE_METHOD(ComponentSeralizerTestFixture, "TestSaveComponent") {
    Position expected1(200.0f, 100.0f);
    Json::Value v;
    v["pos"] = Serializable::toJSON(expected1);
    ComponentSerializer cs0;
    REQUIRE(cs0.toString() == "null\n");
    Json::Value actual = cs0.Save(expected1);
    REQUIRE(actual == v);
}

TEST_CASE_METHOD(ComponentSeralizerTestFixture, "TestLoadFromStream") {
    ComponentSerializer cs0;
    REQUIRE(cs0.toString() == "null\n");

    Position p2(20000.135f, 500.45f);
    Json::Value v = cs0.Save<Position>(p2);
    std::istringstream stream(toString(v));
    int ret = ComponentSerializer::LoadFromStream(stream, cs0);
    REQUIRE(cs0.toString() == toStyledString(v));
    REQUIRE(ret == 1);
    REQUIRE(cs0.toString() != "null\n");

    Position loaded;
    loaded = cs0.Load<Position>();
    REQUIRE(loaded == p2);
    // Test Percision hasn't been lost.
    REQUIRE(loaded.x == p2.x);
    REQUIRE(loaded.y == p2.y);
}

// TODO(SMA) : Figure out how to test me.
// TEST_CASE_METHOD(ComponentSeralizerTestFixture, "TestLoadFromStream") {
// }

TEST_CASE_METHOD(ComponentSeralizerTestFixture, "TestLoadAndAssignToEntity") {
    REQUIRE(em.size() == 0UL);

    Position p1(20000.135f, 500.45f);
    ComponentSerializer cs0;
    Json::Value v = cs0.Save<Position>(p1);
    ComponentSerializer cs1(v);
    REQUIRE(cs1.toString() == toStyledString(v));

    SECTION("Assign to valid entity") {
        Entity e = em.create();
        REQUIRE(e.valid());
        REQUIRE(em.size() == 1UL);
        REQUIRE(0 == size(em.entities_with_components<Position>()));
        ComponentSerializer::LoadComponentToEntity<Position>(cs1, e);
        REQUIRE(1 == size(em.entities_with_components<Position>()));
        REQUIRE(static_cast<bool>(e.component<Position>()));
        auto pos = e.component<Position>();
        REQUIRE(*pos.get() == p1);
        e.destroy();
    }
    SECTION("Assign to valid entity with position") {
        Entity e = em.create();
        REQUIRE(e.valid());
        REQUIRE(em.size() == 1UL);
        Position p2(100.0f, -100.0f);
        e.assign<Position>(p2);

        REQUIRE(1 == size(em.entities_with_components<Position>()));
        ComponentSerializer::LoadComponentToEntity<Position>(cs1, e);
        REQUIRE(1 == size(em.entities_with_components<Position>()));
        REQUIRE(static_cast<bool>(e.component<Position>()));
        auto pos = e.component<Position>();
        REQUIRE(*pos.get() == p1);
        e.destroy();
    }

    SECTION("Assign from empty serializer") {
        ComponentSerializer cs2;
        REQUIRE(cs2.toString() == "null\n");
        Entity e = em.create();
        REQUIRE(e.valid());
        REQUIRE(em.size() == 1UL);
        REQUIRE(0 == size(em.entities_with_components<Position>()));
        ComponentSerializer::LoadComponentToEntity<Position>(cs2, e);
        REQUIRE(0 == size(em.entities_with_components<Position>()));
        e.destroy();
    }
}

TEST_CASE_METHOD(ComponentSeralizerTestFixture, "TestSaveEntity") {
    REQUIRE(em.size() == 0UL);

    Position p1(20000.135f, 500.45f);
    Entity e = em.create();
    e.assign_from_copy<Position>(p1);
    REQUIRE(e.valid());
    REQUIRE(em.size() == 1UL);

    ComponentSerializer cs1;
    Json::Value v = ComponentSerializer::SaveEntityComponent<Position>(cs1, e);
    Json::Value expected = cs1.Save<Position>(p1);
    REQUIRE(toString(expected) == toString(v));
    e.destroy();
}
