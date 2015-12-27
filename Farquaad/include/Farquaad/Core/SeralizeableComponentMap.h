// Copyright 2015 Bablawn3d5

#pragma once

#include <json/json.h>
#include <entityx/entityx.h>
#include <Farquaad/Core/ComponentSerializer.h>

// Maps component rootnames to ComponentSerializer
class SeralizeableComponentMap {
public:
    typedef std::function<void(const ComponentSerializer& cs, ex::Entity&)> AssignentFunction;
    typedef std::map<const std::string, AssignentFunction > MapType;
    MapType nameToAssignor;

    SeralizeableComponentMap() = default;
    ~SeralizeableComponentMap() = default;

    // Register a new assignor to the map.
    void Register(const std::string& name, AssignentFunction serlize_func);

    // Returns true if key is regsitered with this map.
    bool isRegistered(const std::string& key);

    // Creates a entity given the string
    void Create(const std::string name, const ComponentSerializer& cs,
                ex::Entity& e) const;

    // Returns global instance of the SerlizedComponentMap
    static SeralizeableComponentMap & get() {
        static SeralizeableComponentMap map;
        return map;
    }
};

// Class that defines that the class is registered to the global ComponentMap
// this class should always be implemented as a static member of SerializableHandle.
// It can be treated almost as a string.
template<class T>
class RegisteredSerializableComponent {
public:
    const std::string rootName;
    RegisteredSerializableComponent(std::string rootName) : rootName(rootName) {
        SeralizeableComponentMap::get().Register(rootName,
                                                 &ComponentSerializer::LoadAndAssignToEntity<T>);
    }

    operator const std::string&() const { return rootName; }
};

// Class definition for SerializableHandle that should be specialized by each
// component.
//
// See JSONSerialziedComponents.h for component specializations
// See JSONSerialiedPrimitiveTypes.hpp for primitive specializations
template<class T>
class SerializableHandle {
public:
    static const RegisteredSerializableComponent<T> rootName;

    // To be overwritten by template specializations
    // If you get an error here saying your instantiating a abstract
    // class. It means you should specailize SerializableHandle<foo> somewhere.
    virtual T fromJSON(const Json::Value&) const = 0;
    virtual Json::Value toJSON(const T& component) const = 0;
};