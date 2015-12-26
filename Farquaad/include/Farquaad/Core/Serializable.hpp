// Copyright 2015 Bablawn3d5

#pragma once

#include <json/json.h>
#include <string>
#include <functional>

// See SeralizeableComponentMap.h
template<class T> class SerializableHandle;

// Static handle to SerializableHandle
template<typename T>
class Serializable {
public:
    static inline T fromJSON(const Json::Value& json) {
        return handle().fromJSON(json);
    }

    static inline Json::Value toJSON(const T& component) {
        return handle().toJSON(component);
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
        getValueByRootName(ret) = handle().toJSON(component);
        return ret;
    }

private:
    static SerializableHandle<T>& handle() {
        static SerializableHandle<T> handle;
        return handle;
    }
};
