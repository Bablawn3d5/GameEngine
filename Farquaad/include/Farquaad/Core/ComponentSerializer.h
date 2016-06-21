// Copyright 2015-2016 Bablawn3d5
#pragma once

#include <Farquaad/Core/Serializable.hpp>
#include <Meta.h>
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
    bool HasComponent() const;

    template<typename T>
    T Load() const; // NOLINT

    template<typename T>
    Json::Value Save(ex::ComponentHandle<T>& component) const; // NOLINT

    template<typename T>
    Json::Value Save(const T& component) const;

    template<typename T>
    static inline void LoadComponentToEntity(const ComponentSerializer & cs, ex::Entity& e); // NOLINT

    template<typename T>
    static inline Json::Value SaveEntityComponent(const ComponentSerializer & cs, ex::Entity& e); // NOLINT

    const std::string toString() const;

    int ParseEntityString(const std::string str);
    static int LoadFromFile(const std::string & filename, ComponentSerializer & cs); // NOLINT
    static int LoadFromStream(const std::istream & stream, ComponentSerializer & cs); // NOLINT

private:
    Json::Value value;
};

template<typename T>
inline bool ComponentSerializer::HasComponent() const {
    return value[meta::getName<T>()].isObject();
}

template<typename T>
inline T ComponentSerializer::Load() const {
    return Serializable::fromJSON<T>(value[meta::getName<T>()]);
}

template<typename T> inline
Json::Value ComponentSerializer::Save(ex::ComponentHandle<T>& handle) const {
    return this->Save(*handle.get());
}

template<typename T> inline
Json::Value ComponentSerializer::Save(const T& component) const {
    Json::Value v;
    v[meta::getName<T>()] = Serializable::toJSON<T>(component);
    return v;
}

template<typename T> inline
void ComponentSerializer::LoadComponentToEntity(const ComponentSerializer & cs, ex::Entity& e) {
    if ( !cs.HasComponent<T>() ) {
        return;
    }
    T component(cs.Load<T>());
    e.replace<T>(component);
}

template<typename T> inline
Json::Value ComponentSerializer::SaveEntityComponent(const ComponentSerializer & cs, ex::Entity& e) {
    Json::Value v;

    if ( e.has_component<T>() ) {
        ex::ComponentHandle<T> handle = e.component<T>();
        auto json = cs.Save<T>(handle);
        v[meta::getName<T>()] = json[meta::getName<T>()];
    }

    return v;
}
