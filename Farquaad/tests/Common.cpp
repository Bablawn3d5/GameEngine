// Copyright 2016 Bablawn3d5

#include <Farquaad/Serialization.hpp>
#include <Farquaad/Core.hpp>
#include <Farquaad/Core/ComponentSerializer.h>
#include <Farquaad/Core.hpp>
#include <entityx/entityx.h>
#include <Common.h>

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

bool operator==(const KeyInput& a, const KeyInput& b) {
    return a.userInputType == b.userInputType &&
        a.keyCode == b.keyCode &&
        a.mouseButton == b.mouseButton &&
        a.eventType == b.eventType;
}

bool operator!=(const KeyInput& a, const KeyInput& b) {
    return !(a == b);
}

bool operator==(const InputSystem::KeyBindMap& a, const InputSystem::KeyBindMap& b) {
    for ( auto& pair : a ) {
        std::string s = pair.first;
        if ( b.at(s) != a.at(s) )
            return 0;
    }

    for ( auto& pair : b ) {
        std::string s = pair.first;
        if ( b.at(s) != a.at(s) )
            return 0;
    }

    return 1;
}

std::ostream &operator<<(std::ostream &out, const Position &position) {
    out << "Position(" << position.x << ", " << position.y << ")";
    return out;
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

std::ostream &operator<<(std::ostream &out, const KeyInput &v) {
    out << "KeyInput(" << v.userInputType << ", " <<
        v.keyCode << ", " <<
        v.mouseButton << ", " <<
        v.eventType << ")";
    return out;
}

std::ostream &operator<<(std::ostream &out, const InputSystem::KeyBindMap& a) {
    for ( auto& pair : a ) {
        std::string s = pair.first;
        out << "Key( " << s << " ) : " << pair.second << std::endl;
    }
    return out;
}

std::string toString(const Json::Value &value) {
    Json::FastWriter w;
    return w.write(value);
}

std::string toStyledString(const Json::Value &value) {
    std::stringstream stream;
    Json::StyledStreamWriter writer;
    writer.write(stream, value);
    return stream.str();
}
