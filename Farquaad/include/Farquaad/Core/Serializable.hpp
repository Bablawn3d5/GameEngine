// Copyright 2015 Bablawn3d5

#pragma once

#include <json/json.h>
#include <memory>
#include <map>
#include <string>
#include <cassert>

// Class definition for SerializableHandle that should be specialized by each
// component.
//
// See JSONSerialziedComponents.h for component specializations
// See JSONSerialiedPrimitiveTypes.hpp for primitive specializations
template<class T>
class SerializableHandle {
public:
    virtual T fromJSON(const Json::Value&) const = 0;
    virtual Json::Value toJSON(const T& component) const = 0;
};

// Static handle to SerializableHandle
class Serializable {
public:
    template<typename T>
    static inline T fromJSON(const Json::Value& json) {
        return handle<T>().fromJSON(json);
    }

    template<typename T>
    static inline Json::Value toJSON(const T& component) {
        return handle<T>().toJSON(component);
    }

    // If you get an error here saying your instantiating a abstract
    // class foo. It means you should specailize SerializableHandle<foo>
    // somewhere.
    template<typename T>
    static const SerializableHandle<T>& handle() {
        static_assert(!std::is_abstract<SerializableHandle<T>>::value,
                      "SerializableHandle should be implemented somehwere");
        static_assert(std::is_default_constructible<SerializableHandle<T>>::value,
                      "SerializableHandle should have a defualt constuctor");
        static const SerializableHandle<T> handle;
        return handle;
    }
};

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

    // Assigns value from v to C's pointed member of type V
    inline void fromJSON(const Json::Value& v, C& obj) const {
        (&obj)->*memberPtr = Serializable::fromJSON<V>(v);
    }

    // Serializes C's member of type V to JSON.
    inline Json::Value toJSON(const C& obj) const {
        return Serializable::toJSON((V&)((&obj)->*memberPtr));
    }
};

template<class C>
class SerializeFromRegistry {
public:
    // HACK(SMA) : For some reason uniuqe_ptr doesn't work too well here
    // something about the copy constructor of pair? For now use shared_ptr.
    // Although these pointers arn't meant to be shared.
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
        static_assert(!std::is_abstract<SerializableHandle<V>>::value,
                      "SerializableHandle for memberPtr type V should be implemented somehwere");
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
