// Copyright 2015 Bablawn3d5

// This file contains SerializableHandle(s) for primitive built in C++ types.
// if you wish to implement a handle see JSONSerializedComponent.hpp

#pragma once

#include <json/json.h>
#include <string>

template<>
class SerializableHandle<bool> {
public:
    // explicitly no rootName. Do not use float as root element.
    // Makes no sense.

    inline bool fromJSON(const Json::Value& v) const {
        return v.asBool();
    }

    inline Json::Value toJSON(const bool& primitive) const {
        return Json::Value(primitive);
    }
};

template<>
class SerializableHandle<float> {
public:
    // explicitly no rootName. Do not use float as root element.
    // Makes no sense.

    inline float fromJSON(const Json::Value& v) const {
        return v.asFloat();
    }

    inline Json::Value toJSON(const float& primitive) const {
        return Json::Value(primitive);
    }
};

template<>
class SerializableHandle<int> {
public:
    // explicitly no rootName. Do not use int as root element.
    // Makes no sense.

    inline int fromJSON(const Json::Value& v) const {
        return v.asInt();
    }

    inline Json::Value toJSON(const int& primitive) const {
        return Json::Value(primitive);
    }
};

template<>
class SerializableHandle<std::string> {
public:
    // explicitly no rootName. Do not use string as root element.
    // Makes no sense.

    inline std::string fromJSON(const Json::Value& v) const {
        return v.asString();
    }

    inline Json::Value toJSON(const std::string& primitive) const {
        return Json::Value(primitive);
    }
};
