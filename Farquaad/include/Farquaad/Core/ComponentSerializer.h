// Copyright 2015-2016 Bablawn3d5
#pragma once

#include <Farquaad/Core/Serializable.hpp>
#include <json/json.h>
#include <entityx/entityx.h>
#include <string>
#include <sstream>

namespace ex = entityx;

template<typename T> class MappedComponent;

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
    const MappedComponent<T>& handle = Serializable::handle<T>();
    return value[handle.rootName].isObject();
}

template<typename T>
inline T ComponentSerializer::Load() const {
    const MappedComponent<T>& handle = Serializable::handle<T>();
    return Serializable::fromJSON<T>(value[handle.rootName]);
}

template<typename T> inline
Json::Value ComponentSerializer::Save(ex::ComponentHandle<T>& handle) const {
    return this->Save(*handle.get());
}

template<typename T> inline
Json::Value ComponentSerializer::Save(const T& component) const {
    const MappedComponent<T>& handle = Serializable::handle<T>();
    Json::Value v;
    v[handle.rootName] = Serializable::toJSON<T>(component);
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
        const MappedComponent<T>& mappedhandle = Serializable::handle<T>();
        ex::ComponentHandle<T> handle = e.component<T>();
        auto json = cs.Save<T>(handle);
        v[mappedhandle.rootName] = json[mappedhandle.rootName];
    }

    return v;
}
