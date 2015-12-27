// Copyright 2015 Bablawn3d5

#pragma once

#include <json/json.h>
#include <memory>
#include <map>
#include <string>

// Class definition for SerializableHandle that should be specialized by each
// component.
//
// See JSONSerialziedComponents.h for component specializations
// See JSONSerialiedPrimitiveTypes.hpp for primitive specializations
template<class T>
class SerializableHandle {
public:
    // To be overwritten by template specializations
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

private:
    // If you get an error here saying your instantiating a abstract
    // class foo. It means you should specailize SerializableHandle<foo>
    // somewhere.
    template<typename T>
    static SerializableHandle<T>& handle() {
        static SerializableHandle<T> handle;
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
