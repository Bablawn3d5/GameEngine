// Copyright 2015 Bablawn3d5

#pragma once

#include <json/json.h>
#include <string>

template<typename T>
class SerializableHandle {
public:
    static const std::string rootName;
    SerializableHandle() {}
    ~SerializableHandle<T>() {}

    T fromJSON(const Json::Value&) const = 0;
    Json::Value toJSON(const T& component) const = 0;
};

// Defines that a component can be serialized by a ComponentSerializer
template<typename T>
class Serializable {
public:
    Serializable() {}
    ~Serializable<T>() {}

    static inline T fromJSON(const Json::Value& json) {
        SerializableHandle<T> handle;
        return handle.fromJSON(json);
    }

    static inline Json::Value toJSON(const T& component) {
        SerializableHandle<T> handle;
        return handle.toJSON(component);
    }

    static inline const Json::Value& getValueByRootName(const Json::Value& value) {
        std::string componentName = SerializableHandle<T>::rootName;
        return value[componentName];
    }

    static inline Json::Value& getValueByRootName(Json::Value& value) {
        std::string componentName = SerializableHandle<T>::rootName;
        return value[componentName];
    }

    static inline Json::Value writeValueToRootName(const T& component) {
        Json::Value ret;
        SerializableHandle<T> handle;
        getValueByRootName(ret) = handle.toJSON(component);
        return ret;
    }
};
