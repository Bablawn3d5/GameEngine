// Copyright 2015 Bablawn3d5
#define CATCH_CONFIG_MAIN

#include <Farquaad/Core.hpp>
#include <Farquaad/Core/SeralizeableComponentMap.h>
#include <entityx/entityx.h>
#include <string>
#include <sstream>
#include <iostream>
// TODO(SMA): Fix these includes...
#include "entityx/3rdparty/catch.hpp"

using entityx::Entity;
using entityx::EventManager;
using entityx::EntityManager;

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
class SerializableHandle<Position> : public MappedComponent<Position> {
public:
    SerializableHandle() : MappedComponent("pos") {}

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

// LEGACY(SMA) : Remove me! We shouldn't need this here as component seralizer should
// handle this in some way.
template<typename T>
static inline Json::Value writeValueToRootName(const T& component) {
    Json::Value ret;
    const MappedComponent<T>& handle = Serializable::handle<T>();
    ret[handle.rootName] = Serializable::toJSON(component);
    return ret;
}

std::ostream &operator<<(std::ostream &out, const Position &position) {
    out << "Position(" << position.x << ", " << position.y << ")";
    return out;
}

std::string toString(const Json::Value &value) {
    std::stringstream stream;
    Json::StyledStreamWriter writer;
    writer.write(stream, value);
    return stream.str();
}

struct ComponentSeralizerTestFixture {
    ComponentSeralizerTestFixture() : em(ev) {}
    EntityManager em;
    EventManager ev;
};

TEST_CASE_METHOD(ComponentSeralizerTestFixture, "TestDefaultConstructor") {
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
    REQUIRE(cs1.toString() == toString(expected));

    // Is copy.
    root2["bar"] = 2;
    REQUIRE(cs1.toString() == toString(expected));
}

TEST_CASE_METHOD(ComponentSeralizerTestFixture, "TestParseEntityString") {
    ComponentSerializer cs0;
    REQUIRE(cs0.toString() == "null\n");

    SECTION("Load good string") {
        int ret = cs0.ParseEntityString("{\"foo\" : \"bar\"}");
        REQUIRE(ret == 1);
        Json::Value root;
        root["foo"] = "bar";
        REQUIRE(cs0.toString() == toString(root));
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

TEST_CASE_METHOD(ComponentSeralizerTestFixture, "TestLoadComponent") {
    ComponentSerializer cs0;
    REQUIRE(cs0.toString() == "null\n");

    // Load works from empty
    Position p(1.0f, 2.0f);
    cs0.Load<Position>(p);
    Position expected(0.0f, 0.0f);
    REQUIRE(p == expected);

    // Load works with values loaded
    Position expected1(250.99999999f, 210.5f);
    Position p1;
    Json::Value v;
    v["pos"] = Serializable::toJSON(expected1);
    ComponentSerializer cs1(v);
    cs1.Load(p1);
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
    Json::Value v = writeValueToRootName(p2);
    std::istringstream stream(toString(v));
    int ret = ComponentSerializer::LoadFromStream(stream, cs0);
    REQUIRE(cs0.toString() == toString(v));
    REQUIRE(ret == 1);
    REQUIRE(cs0.toString() != "null\n");

    Position loaded;
    cs0.Load(loaded);
    REQUIRE(loaded == p2);
    // Test Percision hasn't been lost.
    REQUIRE(loaded.x == p2.x);
    REQUIRE(loaded.y == p2.y);
}

TEST_CASE_METHOD(ComponentSeralizerTestFixture, "TestLoadAndAssignToEntity") {
    REQUIRE(em.size() == 0UL);

    Position p1(20000.135f, 500.45f);
    Json::Value v = writeValueToRootName(p1);
    ComponentSerializer cs1(v);
    REQUIRE(cs1.toString() == toString(v));

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
}

// TODO(SMA) : Finish me.
// TEST_CASE_METHOD(ComponentSeralizerTestFixture, "LoadFromFile") {
// }
