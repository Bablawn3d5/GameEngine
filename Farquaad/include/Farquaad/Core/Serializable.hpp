// Copyright 2016 Bablawn3d5

#pragma once

#include <boost/python.hpp>
#include <Farquaad/Systems/PythonSystem.h>
#include <Farquaad/Core/MetaRegister.hpp>
#include <Meta.h>
#include <json/json.h>
#include <memory>
#include <map>
#include <string>
#include <cassert>

namespace py = boost::python;

// Forward declare these.
template<typename> class SerializableHandle;
namespace Serializable {
template<typename T> T fromJSON(const Json::Value&);
template<typename T> Json::Value toJSON(const T&);
template<typename T> void initPy(py::class_<T>&);
template<typename T> const SerializableHandle<T>& handle();
}

// Class definition for SerializableHandle that should be specialized by each
// component.
template<class T>
class SerializableHandle {
public:
  T fromJSON(const Json::Value& v) const {
    T obj;
    if ( v.isNull() ) { return obj; }
    meta::doForAllMembers<T>(
      [&v, &obj](const auto& member) {
      using memeber_type = meta::get_member_type<decltype(member)>;
      const auto& name = member.getName();
      member.set(obj, Serializable::fromJSON<memeber_type>(v[name]));
    });
    return obj;
  }

  Json::Value toJSON(const T& component) const {
    Json::Value v = Json::objectValue;
    //for ( const auto& pair : members ) {
    //  const auto& name = pair.first;
    //  const auto& memberPtr = pair.second;
    //  v[name] = memberPtr->toJSON(obj);
    //}
    meta::doForAllMembers<T>(
      [&v, &component](const auto& member) {
      using memeber_type = meta::get_member_type<decltype(member)>;
      const auto& name = member.getName();
      v[name] = Serializable::toJSON<memeber_type>(member.get(component));
    });
    return v;
  }

  void initPy(py::class_<T>& py) const {
    meta::doForAllMembers<T>(
      [&py](const auto& member) {
      using memeber_type = meta::get_member_type<decltype(member)>;
      const auto& name = member.getName();
      py.def_readwrite(name, member.getPtr());
    });
    py.def("assign_to", &assign_to<T>)
      .def("get_component", &get_component<T>,
           py::return_value_policy<py::reference_existing_object>())
      .staticmethod("get_component");
  }
};

// Static handle to SerializableHandle
namespace Serializable {
template<typename T>
inline T fromJSON(const Json::Value& json) {
    return handle<T>().fromJSON(json);
}

template<typename T>
inline Json::Value toJSON(const T& component) {
    return handle<T>().toJSON(component);
}

template<typename T>
inline void initPy(py::class_<T>& py) {
    handle<T>().initPy(py);
}

// If you get an error here saying your instantiating a abstract
// class foo. It means you should specailize SerializableHandle<foo>
// somewhere.
template<typename T>
const SerializableHandle<T>& handle() {
    static_assert(!std::is_abstract<SerializableHandle<T>>::value,
                  "SerializableHandle should be implemented somehwere");
    static_assert(std::is_default_constructible<SerializableHandle<T>>::value,
                  "SerializableHandle should have a defualt constuctor");
    static const SerializableHandle<T> handle;
    return handle;
}
}

#include <Farquaad/Core/SerializableHandle.hpp>
