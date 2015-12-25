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
