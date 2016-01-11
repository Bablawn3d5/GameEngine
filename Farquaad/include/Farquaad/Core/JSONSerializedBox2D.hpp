// Copyright 2016 Bablawn3d5

#pragma once

#include <Farquaad/Core/Serializable.hpp>
#include <Box2D/Box2D.h>
#include <json/json.h>
#include <string>

template<>
class SerializableHandle<b2BodyType> {
public:
    // Workaround: DR253: http://www.open-std.org/jtc1/sc22/wg21/docs/cwg_active.html#253
    // Define these so class becomes non POD for const initalizaiton
    SerializableHandle() {}
    ~SerializableHandle() {}

    inline b2BodyType fromJSON(const Json::Value &v) const {
        if ( v.asString().compare("STATIC") == 0 ) {
            return b2_staticBody;
        } else if ( v.asString().compare("DYNAMIC") == 0 ) {
            return b2_dynamicBody;
        } else if ( v.asString().compare("KINEMATIC") == 0 ) {
            return b2_kinematicBody;
        }

        // HACK(SMA) : Just return static???
        return b2_staticBody;
    }

    inline Json::Value toJSON(const b2BodyType& type) const {
        Json::Value v;
        switch ( type ) {
        case b2_staticBody:
            v = "STATIC";
            break;
        case b2_dynamicBody:
            v = "DYNAMIC";
            break;
        case b2_kinematicBody:
            v = "KINEMATIC";
            break;
        default:
            // HACK(SMA) : Assume is static???
            v = "STATIC";
        }
        return v;
    }
};
