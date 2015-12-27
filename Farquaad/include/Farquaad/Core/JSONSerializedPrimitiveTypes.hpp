// Copyright 2015 Bablawn3d5

// This file contains SerializableHandle(s) for primitive built in C++ types.

#pragma once

#include <json/json.h>
#include <string>

class SerializablePrimitive {
public:
    template<class T>
    inline Json::Value toJSON(const T& primitive) const {
        static_assert(std::is_fundamental<T>::value,
                      "You should only use this on primitive types");
        return Json::Value(primitive);
    }
};

template<>
class SerializableHandle<bool> : public SerializablePrimitive {
public:
    inline bool fromJSON(const Json::Value& v) const {
        return v.asBool();
    }
};

template<>
class SerializableHandle<float> : public SerializablePrimitive {
public:
    inline float fromJSON(const Json::Value& v) const {
        return v.asFloat();
    }
};

template<>
class SerializableHandle<double> : public SerializablePrimitive {
public:
    inline double fromJSON(const Json::Value& v) const {
        return v.asDouble();
    }
};
template<>
class SerializableHandle<int> : public SerializablePrimitive {
public:
    inline int fromJSON(const Json::Value& v) const {
        return v.asInt();
    }
};

template<>
class SerializableHandle<const char*> : public SerializablePrimitive {
public:
    inline const char* fromJSON(const Json::Value& v) const {
        return v.asCString();
    }
};

template<>
class SerializableHandle<std::string> : public SerializablePrimitive {
public:
    inline std::string fromJSON(const Json::Value& v) const {
        return v.asString();
    }
};
