// Copyright 2016 Bablawn3d5
#include <Farquaad/Core.hpp>
#include <entityx/entityx.h>
#include <string>
#include <sstream>
#include <iostream>
#include <catch.hpp>

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

bool operator==(const Body &r, const Body &l) {
    return r.direction == l.direction &&
        r.position == r.position;
}

bool operator==(const Stats &r, const Stats &l) {
    return r.currentHp == l.currentHp &&
        r.maxHp == l.maxHp &&
        r.speed == l.speed &&
        r.godmode == l.godmode;
}

std::string toString(const Json::Value &value) {
    std::stringstream stream;
    Json::StyledStreamWriter writer;
    writer.write(stream, value);
    return stream.str();
}

struct EntitySeralizerTestFixture {
    EntitySeralizerTestFixture() : em(ev) {}
    EntityManager em;
    EventManager ev;
};

TEST_CASE_METHOD(EntitySeralizerTestFixture, "TestDefaultConstructor") {
    EntitySerializer es;
    REQUIRE(es.toString() == "null\n");
}

TEST_CASE_METHOD(EntitySeralizerTestFixture, "TestSave") {
    Body b{ 10.0f, 19.99f, -20.0f, 30.2451f };
    Stats s{ 100.999f, 200000.0f, -20000.0f };
    InputResponder r;
    EntitySerializer es;
    REQUIRE(es.toString() == "null\n");

    REQUIRE(em.size() == 0UL);
    Entity e = em.create();
    REQUIRE(e.valid());
    REQUIRE(em.size() == 1UL);
    REQUIRE(0 == size(em.entities_with_components<Body>()));
    REQUIRE(0 == size(em.entities_with_components<Stats>()));
    REQUIRE(0 == size(em.entities_with_components<InputResponder>()));
    e.assign_from_copy<Body>(b);
    e.assign_from_copy<Stats>(s);
    e.assign_from_copy<InputResponder>(r);
    REQUIRE(1 == size(em.entities_with_components<Body>()));
    REQUIRE(1 == size(em.entities_with_components<Body>()));
    REQUIRE(1 == size(em.entities_with_components<InputResponder>()));

    Json::Value expected;
    expected["body"] = Serializable::toJSON<Body>(b);
    expected["stats"] = Serializable::toJSON<Stats>(s);
    expected["inputresponder"] = Serializable::toJSON<InputResponder>(r);
    Json::Value actual = es.Save(e);
    REQUIRE(expected == actual);
    e.destroy();
}

TEST_CASE_METHOD(EntitySeralizerTestFixture, "TestLoad") {
    Body b{ 10.0f, 19.99f, -20.0f, 30.2451f };
    Stats s{ 100.999f, 200000.0f, -20000.0f };

    REQUIRE(em.size() == 0UL);
    Entity e = em.create();
    REQUIRE(e.valid());
    REQUIRE(em.size() == 1UL);
    REQUIRE(0 == size(em.entities_with_components<Body>()));
    REQUIRE(0 == size(em.entities_with_components<Stats>()));

    Json::Value expected;
    expected["body"] = Serializable::toJSON<Body>(b);
    expected["stats"] = Serializable::toJSON<Stats>(s);
    EntitySerializer es(expected);
    REQUIRE(es.toString() == toString(expected));
    es.Load(e);
    REQUIRE(1 == size(em.entities_with_components<Body>()));
    REQUIRE(1 == size(em.entities_with_components<Stats>()));
    REQUIRE(*e.component<Body>().get() == b);
    REQUIRE(*e.component<Stats>().get() == s);
    e.destroy();
}
