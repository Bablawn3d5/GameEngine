// Copyright 2015 Bablawn3d5

#pragma once

#include <Farquaad/Core/SeralizeableComponentMap.h>
#include <Farquaad/Core/JSONSerializedPrimitiveTypes.hpp>
#include <Farquaad/Components.hpp>
#include <json/json.h>
#include <map>
#include <string>

// Base class for Member serialization
template<typename C>
class MemberBase {
public:
    virtual ~MemberBase() = default;

    virtual void fromJSON(const Json::Value&, C&) const = 0;
    virtual Json::Value toJSON(const C&) const = 0;
};

// Defines serialization for a particular member
template<typename V, typename C>
class Member : public MemberBase<C> {
public:
    typedef V C::* Ptr;
    Ptr memberPtr;

    explicit Member(Ptr memberPtr) : memberPtr(memberPtr) {}

    inline void fromJSON(const Json::Value& v, C& obj) const {
        (&obj)->*memberPtr = Serializable::fromJSON<V>(v);
    }

    inline Json::Value toJSON(const C& obj) const {
        return Serializable::toJSON((V&)((&obj)->*memberPtr));
    }
};

template<class C>
class SerializeFromRegistry {
public:
    typedef std::shared_ptr<MemberBase<C>> MemberBasePtr;
    typedef std::map<std::string, MemberBasePtr> MemberMap;

    explicit SerializeFromRegistry(const MemberMap& map) : members(map) {}
    virtual ~SerializeFromRegistry() {}

    virtual const MemberMap GenerateMap() = 0;

    template<class K, class V>
    static bool isRegistered(const std::map<K, V>& map, const std::string& key) {
        auto it = map.find(key);
        if ( it != map.end() ) {
            return 1;
        }
        return 0;
    }

    template<typename V>
    static inline void AddMember(MemberMap& members,
                                 std::string name, V C::* memberPtr) {
        assert(isRegistered(members, name) == false);
        // Attempt to workaround unique_ptr issue.
        members.insert(std::make_pair(name,
                                      MemberBasePtr(new Member<V, C>(memberPtr))));
    }

    inline C fromJSON(const Json::Value& v) const {
        C obj;
        for ( const auto& pair : members ) {
            const auto& name = pair.first;
            const auto& memberPtr = pair.second;
            memberPtr->fromJSON(v[name], obj);
        }
        return obj;
    }

    inline Json::Value toJSON(const C& obj) const {
        Json::Value v;
        for ( const auto& pair : members ) {
            const auto& name = pair.first;
            const auto& memberPtr = pair.second;
            v[name] = memberPtr->toJSON(obj);
        }
        return v;
    }

private:
    MemberMap members;
};

template<typename T>
class SerializableHandle<sf::Vector2<T>> {
public:
    //explicit no root name.

    inline sf::Vector2<T> fromJSON(const Json::Value&) const;
    inline Json::Value toJSON(const sf::Vector2<T>& component) const;
};

template<>
class SerializableHandle<Body> : public SerializeFromRegistry<Body> {
public:
    static const RegisteredSerializableComponent<Body> rootName;
    SerializableHandle() : SerializeFromRegistry<Body>(this->GenerateMap()) {}
    const SerializeFromRegistry<Body>::MemberMap GenerateMap();
};

template<>
class SerializableHandle<Stats> : public SerializeFromRegistry<Stats> {
public:
    static const RegisteredSerializableComponent<Stats> rootName;
    SerializableHandle() : SerializeFromRegistry<Stats>(this->GenerateMap()) {}
    const SerializeFromRegistry<Stats>::MemberMap GenerateMap();
};

#include <Farquaad/Core/JSONSerializedComponents.hpp> //NOLINT
