// Copyright 2015 Bablawn3d5

#pragma once

#include <json/json.h>
#include <string>
#include <functional>

// See SeralizeableComponentMap.h
template<class T> class SerializableHandle;

// Static handle to SerializableHandle
class Serializable {
public:
    template<typename T>
    static inline T fromJSON(const Json::Value& json) {
        return handle<T>().fromJSON(json);
    }

    template<typename T>
    static inline Json::Value toJSON(const T& component) {
        return handle<T>().toJSON(component);
    }

    template<typename T>
    static inline const Json::Value& getValueByRootName(const Json::Value& value) {
        std::string componentName = SerializableHandle<T>::rootName;
        return value[componentName];
    }

    template<typename T>
    static inline Json::Value& getValueByRootName(Json::Value& value) {
        std::string componentName = SerializableHandle<T>::rootName;
        return value[componentName];
    }

    template<typename T>
    static inline Json::Value writeValueToRootName(const T& component) {
        Json::Value ret;
        getValueByRootName<T>(ret) = handle<T>().toJSON(component);
        return ret;
    }

private:
    template<typename T>
    static SerializableHandle<T>& handle() {
        static SerializableHandle<T> handle;
        return handle;
    }
};
