// Copyright 2015 Bablawn3d5
#pragma once

#include <Farquaad/Core/Serializable.hpp>
#include <json/json.h>
#include <entityx/entityx.h>
#include <string>
#include <sstream>

namespace ex = entityx;

class ComponentSerializer {
public:
    explicit ComponentSerializer(Json::Value value = Json::Value()) : value(value) {}
    ~ComponentSerializer() {}

    template<typename T>
    void Load(T& component) const;

    template<typename T>
    std::string Save(const T& component) const;

    template<typename T>
    static inline void LoadAndAssignToEntity(const ComponentSerializer & cs, ex::Entity& e);

    const std::string toString() const;

    int ParseEntityString(const std::string str);
    static int LoadFromFile(const std::string & filename, ComponentSerializer & cs);
    static int LoadFromStream(const std::istream & stream, ComponentSerializer & cs);

private:
    Json::Value value;
};

template<typename T>
inline void ComponentSerializer::Load(T& component) const {
    Json::Value val = Serializable::getValueByRootName<T>(value);
    component = Serializable::fromJSON<T>(val);
}

template<typename T>
inline std::string ComponentSerializer::Save(const T& component) const {
    Json::Value root = Serializable::toJSON<T>(component);
    std::stringstream stream;
    stream << root;
    return stream.str();
}

template<typename T>
inline void ComponentSerializer::LoadAndAssignToEntity(const ComponentSerializer & cs, ex::Entity& e) {
    T component;
    cs.Load<T>(component);
    if ( e.has_component<T>() ) {
        e.remove<T>();
    }
    e.assign_from_copy<T>(component);
}
