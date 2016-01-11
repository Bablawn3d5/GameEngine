// Copyright 2015-2016 Bablawn3d5

#pragma once

#include <json/json.h>
#include <entityx/entityx.h>
#include <Farquaad/Core/ComponentSerializer.h>

namespace ex = entityx;

// Maps component rootnames to ComponentSerializer
class SeralizeableComponentMap {
public:
    typedef std::function<void(const ComponentSerializer& cs, ex::Entity&)> LoadFunciton;
    typedef std::function<Json::Value(const ComponentSerializer& cs, ex::Entity&)> SaveFunction;
    typedef std::map<const std::string, LoadFunciton > LoadMap;
    typedef std::map<const std::string, SaveFunction > SaveMap;
    LoadMap nameToLoadFunc;
    SaveMap nameToSaveFunc;

    SeralizeableComponentMap() = default;
    ~SeralizeableComponentMap() = default;

    // Register a new assignor to the map.
    void RegisterLoad(const std::string& name, LoadFunciton serlize_func);
    void RegisterSave(const std::string& name, SaveFunction serlize_func);

    // Returns true if key is regsitered with this map.
    bool isRegistered(const std::string& key) {
        bool is_loaded = false;
        bool is_saved = false;

        auto it = nameToLoadFunc.find(key);
        if ( it != nameToLoadFunc.end() ) {
            is_loaded = true;
        }

        auto its = nameToSaveFunc.find(key);
        if ( its != nameToSaveFunc.end() ) {
            is_saved = true;
        }
        return is_loaded && is_saved;
    }

    // Returns global instance of the SeralizeableComponentMap
    static SeralizeableComponentMap & get() {
        static SeralizeableComponentMap map;
        return map;
    }
};

// Class that defines that the class is registered to the global ComponentMap
// It can be treated almost as a string.
template<class T>
class MappedComponent {
public:
    const std::string rootName;
    explicit MappedComponent(const std::string rootName) : rootName(rootName) {
        SeralizeableComponentMap::get().RegisterLoad(rootName,
                                                     &ComponentSerializer::LoadComponentToEntity<T>);
        SeralizeableComponentMap::get().RegisterSave(rootName,
                                                     &ComponentSerializer::SaveEntityComponent<T>);
    }

    operator const std::string&() const { return rootName; }
};
