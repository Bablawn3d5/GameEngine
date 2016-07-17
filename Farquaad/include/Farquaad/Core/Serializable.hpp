// Copyright 2016 Bablawn3d5

#pragma once

#include <boost/python.hpp>
#include <Farquaad/Systems/PythonSystem.h>
#include <Farquaad/Core/MetaRegister.hpp>
#include <Meta.h>
#include <json/json.h>
#include <memory>
#include <string>
#include <cassert>
#include <utility>
#include <unordered_map>

namespace py = boost::python;

// Forward declare these.
template<typename> class SerializableHandle;
namespace Serializable {
template<typename T> T fromJSON(const Json::Value&);
template<typename T> Json::Value toJSON(const T&);
template<typename T> void initPy(py::class_<T>&&);
template<typename T> const SerializableHandle<T>& handle();
}

// Class definition for SerializableHandle that should be specialized by each
// component.
template<class T>
class SerializableHandle {
public:
  // Workaround: DR253: http://www.open-std.org/jtc1/sc22/wg21/docs/cwg_active.html#253
  // Define these so class becomes non POD for const initalizaiton
  SerializableHandle() {}
  ~SerializableHandle() {}

  T fromJSON(const Json::Value& v) const {
    T obj;
    if ( v.isNull() ) { return obj; }
    meta::doForAllMembers<T>(
      [&v, &obj](const auto& member) {
      using memeber_type = meta::get_member_type<decltype(member)>;
      const auto& name = member.getName();
      auto& json_val = v[name];
      if( json_val == Json::nullValue )
        return;
      member.set(obj, Serializable::fromJSON<memeber_type>(json_val));
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

  void initPy(py::class_<T>&& py) const {
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
inline void initPy(py::class_<T>&& py) {
    handle<T>().initPy(std::forward<py::class_<T>>(py));
}

template<typename T>
inline void initPy(py::enum_<T>&& py) {
  handle<T>().initPy(std::forward<py::enum_<T>>(py));
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

template<class T>
class SerializableHandleEnum {
public:
  SerializableHandleEnum(std::unordered_map<std::string, T> mapping) : str_to_enum(mapping){
    for ( auto& pair : mapping ) {
      enum_to_str[pair.second] = pair.first;
    }
  }

  inline T fromJSON(const Json::Value &v) const {
    // Slightly undefined if you don't type it correctly.
    try {
      return str_to_enum.at(v.asString());
    }
    catch ( std::out_of_range ) {};
    
    // Slightly undefined if mapping doesn't exist
    // Item doesn't exist
    return static_cast<T>(0);
  }

  inline Json::Value toJSON(const T& val) const {
    Json::Value v;
    try {
      v = enum_to_str.at(val);
      return v;
    } catch ( std::out_of_range ) {};

    return Json::nullValue;
  }

  inline void initPy(py::class_<T>&& py) const = delete;
  inline void initPy(py::enum_<T>&& pynum) const {
    for ( auto& pair : enum_to_str ) {
      pynum.value(pair.second.c_str(), pair.first);
    }
  }
private:
  std::unordered_map<T, std::string> enum_to_str;
  std::unordered_map<std::string, T> str_to_enum;
};

#include <Farquaad/Core/SerializableHandle.hpp>
