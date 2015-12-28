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
    explicit ComponentSerializer(Json::Value value = Json::nullValue) : value(value) {}
    ~ComponentSerializer() {}

    template<typename T>
    void Load(T& component) const;

    template<typename T>
    Json::Value Save(const T& component) const;

    template<typename T>
    static inline void LoadComponentToEntity(const ComponentSerializer & cs, ex::Entity& e);

    template<typename T>
    static inline Json::Value SaveEntityComponent(const ComponentSerializer & cs, ex::Entity& e);

    const std::string toString() const;

    int ParseEntityString(const std::string str);
    static int LoadFromFile(const std::string & filename, ComponentSerializer & cs);
    static int LoadFromStream(const std::istream & stream, ComponentSerializer & cs);

private:
    Json::Value value;
};

template<typename T>
inline void ComponentSerializer::Load(T& component) const {
    const MappedComponent<T>& handle = Serializable::handle<T>();
    component = Serializable::fromJSON<T>(value[handle.rootName]);
}

template<typename T>
inline Json::Value ComponentSerializer::Save(const T& component) const {
    const MappedComponent<T>& handle = Serializable::handle<T>();
    Json::Value v;
    v[handle.rootName] = Serializable::toJSON<T>(component);
    return v;
}

template<typename T>
inline void ComponentSerializer::LoadComponentToEntity(const ComponentSerializer & cs, ex::Entity& e) {
    T component;
    cs.Load<T>(component);
    e.replace<T>(component);
}

template<typename T>
inline Json::Value ComponentSerializer::SaveEntityComponent(const ComponentSerializer & cs, ex::Entity& e) {
    return Json::nullValue;
}
