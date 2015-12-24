// Copyright 2015 Bablawn3d5

#pragma once

#include <json/json.h>
#include <entityx/entityx.h>
#include <Farquaad/Core/ComponentSerializer.h>
#include <string>
#include <map>
#include <functional>
#include <stdexcept>

namespace ex = entityx;
class ComponentSerializer;

template<typename T>
class SerializableHandle {
public:
    static const std::string rootName;

    // To be overitten by template specialziations
    T fromJSON(const Json::Value&) const = 0;
    Json::Value toJSON(const T& component) const = 0;
};

// Static handle to SerializableHandle
template<typename T>
class Serializable {
public:
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

// Maps component rootnames to ComponentSerializer
class SeralizeableComponentMap {
public:
    typedef std::function<void(const ComponentSerializer& cs, ex::Entity&)> AssignentFunction;
    typedef std::map<const std::string, AssignentFunction > MapType;
    MapType nameToAssignor;

    SeralizeableComponentMap() = default;
    ~SeralizeableComponentMap() = default;

    // Register a new assignor to the map.
    void Register(const std::string& name, AssignentFunction serlize_func) {
        nameToAssignor[name] = serlize_func;
    }

    bool isRegistered(const std::string& key) {
        try {
            nameToAssignor.at(key);
        }
        catch ( std::out_of_range  e ) {
            return 0;
        }
        return 1;
    }

    // Creates a entity given the string
    // TODO(SMA) : Throw a warning or something when requested string is
    // not registered.
    void Create(const std::string name,
                const ComponentSerializer& cs,
                ex::Entity& e) const {
        auto it = nameToAssignor.find(name);
        if ( it != nameToAssignor.end() ) {
            if ( it->second ) {
                it->second(cs, e);
            }
        }
    }

    // Returns global instance of the SerlizedComponentMap
    static SeralizeableComponentMap & get() {
        static SeralizeableComponentMap map;
        return map;
    }
};
