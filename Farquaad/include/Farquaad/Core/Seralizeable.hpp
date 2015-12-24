// Copyright 2015 Bablawn3d5

#pragma once

#include <entityx/entityx.h>
#include <Farquaad/Core/ComponentSeralizer.h>
#include <string>

namespace ex = entityx;

// Defines that a component can be serialized by a ComponentSerializer
template<typename T>
class Seralizeable {
public:
    Seralizeable() {}
    ~Seralizeable<T>() {}

    static inline void LoadFromSeralizer(const ComponentSeralizer& cs, ex::Entity& e) {
        T component;
        cs.Load<T>(&component);
        e.assign_from_copy<T>(component);
    }

    static inline T fromJSON(const Json::Value& json) {
        SeralizeableHandle<T> handle;
        return handle.fromJSON(json);
    };

    static inline Json::Value toJSON(const T& component) {
        SeralizeableHandle<T> handle;
        return handle.toJSON(component);
    };

    static Json::Value getValueByRootName(const Json::Value& value) {
        std::string componentName = SeralizeableHandle<T>::rootName;
        return value[componentName];
    }
};

template<typename T>
class SeralizeableHandle {
public:
    static const std::string rootName;
    SeralizeableHandle() {}
    ~SeralizeableHandle<T>() {}

    T fromJSON(const Json::Value&) const = 0;
    Json::Value toJSON(const T& component) const = 0;
};
