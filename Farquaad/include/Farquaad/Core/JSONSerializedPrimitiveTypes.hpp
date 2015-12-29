// Copyright 2015 Bablawn3d5

// This file contains SerializableHandle(s) for primitive built in C++ types.

#pragma once

#include <Farquaad/Core/Serializable.hpp>
#include <json/json.h>
#include <string>

class SerializablePrimitive {
public:
    // Workaround: DR253: http://www.open-std.org/jtc1/sc22/wg21/docs/cwg_active.html#253
    // Define these so class becomes non POD for const initalizaiton
    SerializablePrimitive() {}
    ~SerializablePrimitive() {}

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
    // Workaround: DR253: http://www.open-std.org/jtc1/sc22/wg21/docs/cwg_active.html#253
    // Define these so class becomes non POD for const initalizaiton
    SerializableHandle() {}
    ~SerializableHandle() {}

    inline bool fromJSON(const Json::Value& v) const {
        return v.asBool();
    }
};

template<>
class SerializableHandle<float> : public SerializablePrimitive {
public:
    // Workaround: DR253: http://www.open-std.org/jtc1/sc22/wg21/docs/cwg_active.html#253
    // Define these so class becomes non POD for const initalizaiton
    SerializableHandle() {}
    ~SerializableHandle() {}

    inline float fromJSON(const Json::Value& v) const {
        return v.asFloat();
    }
};

template<>
class SerializableHandle<double> : public SerializablePrimitive {
public:
    // Workaround: DR253: http://www.open-std.org/jtc1/sc22/wg21/docs/cwg_active.html#253
    // Define these so class becomes non POD for const initalizaiton
    SerializableHandle() {}
    ~SerializableHandle() {}

    inline double fromJSON(const Json::Value& v) const {
        return v.asDouble();
    }
};
template<>
class SerializableHandle<int> : public SerializablePrimitive {
public:
    // Workaround: DR253: http://www.open-std.org/jtc1/sc22/wg21/docs/cwg_active.html#253
    // Define these so class becomes non POD for const initalizaiton
    SerializableHandle() {}
    ~SerializableHandle() {}

    inline int fromJSON(const Json::Value& v) const {
        return v.asInt();
    }
};

template<>
class SerializableHandle<const char*> : public SerializablePrimitive {
public:
    // Workaround: DR253: http://www.open-std.org/jtc1/sc22/wg21/docs/cwg_active.html#253
    // Define these so class becomes non POD for const initalizaiton
    SerializableHandle() {}
    ~SerializableHandle() {}

    inline const char* fromJSON(const Json::Value& v) const {
        return v.asCString();
    }
};

template<>
class SerializableHandle<std::string> : public SerializablePrimitive {
public:
    // Workaround: DR253: http://www.open-std.org/jtc1/sc22/wg21/docs/cwg_active.html#253
    // Define these so class becomes non POD for const initalizaiton
    SerializableHandle() {}
    ~SerializableHandle() {}

    inline std::string fromJSON(const Json::Value& v) const {
        return v.asString();
    }
};
