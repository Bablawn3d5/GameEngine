// Copyright 2015 Bablawn3d5
#pragma once

#include <json/json.h>
#include <Farquaad/Core/Seralizeable.hpp>
#include <array>
#include <string>
#include <sstream>
#include <memory>
#include <vector>

class ComponentSeralizer {
public:
    explicit ComponentSeralizer(Json::Value value = Json::Value()) : value(value) {}
    ~ComponentSeralizer() {}

    template<typename T>
    void Load(T& component) const;

    template<typename T>
    std::string Save(const T& component) const;

    std::string toString();

    int ParseEntityString(const std::string str);
    static int LoadFromFile(const std::string & filename, ComponentSeralizer& cs);
private:
    Json::Value value;
};

template<typename T>
inline void ComponentSeralizer::Load(T& component) const {
    Json::Value val = Seralizeable<T>::getValueByRootName(value);
    component = Seralizeable<T>::fromJSON(val);
}

template<typename T>
inline std::string ComponentSeralizer::Save(const T& component) const {
    Json::Value root = Seralizeable<T>::toJSON(component);
    std::stringstream stream;
    stream << root;
    return stream.str();
}
