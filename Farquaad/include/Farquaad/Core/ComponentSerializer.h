// Copyright 2015 Bablawn3d5
#pragma once

#include <json/json.h>
#include <Farquaad/Core/Serializable.hpp>
#include <entityx/entityx.h>
#include <array>
#include <string>
#include <sstream>
#include <vector>

namespace ex = entityx;

class ComponentSerializer {
public:
    explicit ComponentSerializer(Json::Value value = Json::Value()) : value(value) {}
    ~ComponentSerializer() {}

    template<typename T>
    void Load(T& component) const;

    template<typename T>
    std::string Save(const T& component) const;

    std::string toString();

    int ParseEntityString(const std::string str);
    static int LoadFromFile(const std::string & filename, ComponentSerializer& cs);

    template<typename T>
    static inline void LoadFromSeralizer(const ComponentSerializer& cs, ex::Entity& e) {
        T component;
        cs.Load<T>(&component);
        e.assign_from_copy<T>(component);
    }

private:
    Json::Value value;
};

template<typename T>
inline void ComponentSerializer::Load(T& component) const {
    Json::Value val = Serializable<T>::getValueByRootName(value);
    component = Serializable<T>::fromJSON(val);
}

template<typename T>
inline std::string ComponentSerializer::Save(const T& component) const {
    Json::Value root = Serializable<T>::toJSON(component);
    std::stringstream stream;
    stream << root;
    return stream.str();
}
