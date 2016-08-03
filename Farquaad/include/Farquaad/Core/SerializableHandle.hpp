// Copyright 2016 Bablawn3d5

#pragma once

#include <boost/python.hpp>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>
#include <boost/python/suite/indexing/map_indexing_suite.hpp>
#include <json/json.h>
#include <string>

namespace py = boost::python;

// Built in C++ types.
template <class T>
class SerializableHandlePrimitive {
public:
  // Workaround: DR253: http://www.open-std.org/jtc1/sc22/wg21/docs/cwg_active.html#253
  // Define these so class becomes non POD for const initalizaiton
  SerializableHandlePrimitive() {}
  ~SerializableHandlePrimitive() {}

  inline Json::Value toJSON(const T& primitive) const {
    static_assert(std::is_fundamental<T>::value, "Class only to be used in primitives.");
    return Json::Value(primitive);
  }

};

template<typename T>
class SerializableHandle<sf::Vector2<T>> {
public:
  // Workaround: DR253: http://www.open-std.org/jtc1/sc22/wg21/docs/cwg_active.html#253
  // Define these so class becomes non POD for const initalizaiton
  SerializableHandle() {}
  ~SerializableHandle() {}

  inline sf::Vector2<T> fromJSON(const Json::Value &v) const {
    return sf::Vector2<T>(Serializable::fromJSON<T>(v[0]),
                          Serializable::fromJSON<T>(v[1]));
  }

  inline Json::Value toJSON(const sf::Vector2<T> & component) const {
    Json::Value v = Json::arrayValue;
    v[0] = component.x;
    v[1] = component.y;
    return v;
  }

  inline void initPy(py::class_<sf::Vector2<T>>&& py) {
    py.def_readwrite("x", &sf::Vector2<T>::x)
      .def_readwrite("y", &sf::Vector2<T>::y);
  }
};

template<>
class SerializableHandle<Physics::CoollidingSet> {
public:
  // Workaround: DR253: http://www.open-std.org/jtc1/sc22/wg21/docs/cwg_active.html#253
  // Define these so class becomes non POD for const initalizaiton
  SerializableHandle() {}
  ~SerializableHandle() {}

  // TODO(SMA): This doesn't actually do any conversion. Should fixme sometime.
  Physics::CoollidingSet fromJSON(const Json::Value& v) const {
    Physics::CoollidingSet obj;
    return obj;
  }

  Json::Value toJSON(const Physics::CoollidingSet& arr) const {
    Json::Value v = Json::arrayValue;
    for ( auto& a : arr ) {
      v.append(a.id().index());
    }
    return v;
  }

  void initPy(py::class_<Physics::CoollidingSet>&& py) const {
    py.def(py::vector_indexing_suite<Physics::CoollidingSet, true>());
  }
};

template<class T>
class SerializableHandle<std::vector<T>> {
public:
  // Workaround: DR253: http://www.open-std.org/jtc1/sc22/wg21/docs/cwg_active.html#253
  // Define these so class becomes non POD for const initalizaiton
  SerializableHandle() {}
  ~SerializableHandle() {}

  std::vector<T> fromJSON(const Json::Value& v) const {
    std::vector<T> obj;
    if( !v.isArray() ) { return obj; }
    for ( auto& item : v ) {
      obj.push_back(Serializable::fromJSON<T>(item));
    }
    return obj;
  }

  Json::Value toJSON(const std::vector<T>& arr) const {
    Json::Value v = Json::arrayValue;
    for ( auto& a : arr ) {
      v.append(Serializable::toJSON<T>(a));
    }
    return v;
  }

  void initPy(py::class_<std::vector<T>>&& py) const {
    py.def(py::vector_indexing_suite<std::vector<T>>());
  }
};

template<>
class SerializableHandle<bool> : public SerializableHandlePrimitive<bool> {
public:
  // Workaround: DR253: http://www.open-std.org/jtc1/sc22/wg21/docs/cwg_active.html#253
  // Define these so class becomes non POD for const initalizaiton
  SerializableHandle() {}
  ~SerializableHandle() {}

  inline bool fromJSON(const Json::Value& v) const {
    return v.asBool();
  }
};

template<>
class SerializableHandle<float>
  : public SerializableHandlePrimitive<float> {
public:
  // Workaround: DR253: http://www.open-std.org/jtc1/sc22/wg21/docs/cwg_active.html#253
  // Define these so class becomes non POD for const initalizaiton
  SerializableHandle() {}
  ~SerializableHandle() {}

  inline float fromJSON(const Json::Value& v) const {
    return v.asFloat();
  }
};

template<>
class SerializableHandle<double>
  : public SerializableHandlePrimitive<double> {
public:
  // Workaround: DR253: http://www.open-std.org/jtc1/sc22/wg21/docs/cwg_active.html#253
  // Define these so class becomes non POD for const initalizaiton
  SerializableHandle() {}
  ~SerializableHandle() {}

  inline double fromJSON(const Json::Value& v) const {
    return v.asDouble();
  }
};

// TODO(SMA): Combine these to use is_intergral instead.
template<>
class SerializableHandle<int>
  : public SerializableHandlePrimitive<int> {
public:
  // Workaround: DR253: http://www.open-std.org/jtc1/sc22/wg21/docs/cwg_active.html#253
  // Define these so class becomes non POD for const initalizaiton
  SerializableHandle() {}
  ~SerializableHandle() {}

  inline int fromJSON(const Json::Value& v) const {
    return v.asInt();
  }
};

template<>
class SerializableHandle<size_t> {
public:
  // Workaround: DR253: http://www.open-std.org/jtc1/sc22/wg21/docs/cwg_active.html#253
  // Define these so class becomes non POD for const initalizaiton
  SerializableHandle() {}
  ~SerializableHandle() {}

  inline size_t fromJSON(const Json::Value& v) const {
    return (size_t)v.asUInt();
  }

  inline Json::Value toJSON(size_t v) const {
    return Json::Value(static_cast<Json::UInt>(v));
  }
};

template<>
class SerializableHandle<const char*>
  : public SerializableHandlePrimitive<const char*> {
public:
  // Workaround: DR253: http://www.open-std.org/jtc1/sc22/wg21/docs/cwg_active.html#253
  // Define these so class becomes non POD for const initalizaiton
  SerializableHandle() {}
  ~SerializableHandle() {}

  inline const char* fromJSON(const Json::Value& v) const {
      return v.asCString();
  }
};

template<>
class SerializableHandle<std::string> {
public:
  // Workaround: DR253: http://www.open-std.org/jtc1/sc22/wg21/docs/cwg_active.html#253
  // Define these so class becomes non POD for const initalizaiton
  SerializableHandle() {}
  ~SerializableHandle() {}

  inline Json::Value toJSON(const std::string& s) const {
    return Json::Value(s);
  }

  inline std::string fromJSON(const Json::Value& v) const {
      return v.asString();
  }
};


template<>
class SerializableHandle<PythonScript> {
public:
  // Workaround: DR253: http://www.open-std.org/jtc1/sc22/wg21/docs/cwg_active.html#253
  // Define these so class becomes non POD for const initalizaiton
  SerializableHandle() {}
  ~SerializableHandle() {}

  inline Json::Value toJSON(const PythonScript& s) const {
    Json::Value o;
    // Don't seralize null objects.
    if ( s.object != py::object() ) {
     o["class"] = (std::string)py::extract<std::string>(s.object.attr("__class__").attr("__name__"));
     o["modulename"] = (std::string)
        py::extract<std::string>(s.object.attr("__class__").attr("__module__"));
    } else {
      o["class"] = s.cls;
      o["modulename"] = s.module;
    }
    return o;
  }

  inline PythonScript fromJSON(const Json::Value& v) const {
    return PythonScript(v["modulename"].asCString(), v["class"].asCString());
  }
};

// TODO(SMA) : Replace this with forward declaration instead?
#include <Farquaad/Systems/InputSystem.h>
#include <Thor/Input.hpp>
#include <regex>

template<>
class SerializableHandle<KeyInput> {
public:
  // Workaround: DR253: http://www.open-std.org/jtc1/sc22/wg21/docs/cwg_active.html#253
  // Define these so class becomes non POD for const initalizaiton
  SerializableHandle() {}
  ~SerializableHandle() {}

  inline KeyInput fromJSON(const Json::Value& v) const {
    KeyInput bind;
    auto keyString = v.asString();
    std::regex rek("^KEY_(.*)");
    std::regex rem("^MOUSE_(.*)");
    std::smatch match;
    if ( std::regex_search(keyString, match, rek) && match.size() > 1 ) {
      bind.userInputType = KeyboardInput;
      bind.keyCode = thor::toKeyboardKey(match[1].str());
    } else if ( std::regex_search(keyString, match, rem) && match.size() > 1 ) {
      bind.userInputType = MouseInput;
      bind.mouseButton = thor::toMouseButton(match[1].str());
    } else {
      // Do nothing
      (void)match;
    }
    return bind;
  }

  inline Json::Value toJSON(const KeyInput& bind) const {
    Json::Value v;
    if ( bind.userInputType == KeyboardInput ) {
      v = "KEY_" + thor::toString(bind.keyCode);
    } else if ( bind.userInputType == MouseInput ) {
      v = "MOUSE_" + thor::toString(bind.mouseButton);
    } else {
      // Never should happen.
      assert(!"Something has gone horribly wrong");
    }
    return v;
  }
};

template<>
class SerializableHandle<InputSystem::KeyBindMap> {
public:
  // Workaround: DR253: http://www.open-std.org/jtc1/sc22/wg21/docs/cwg_active.html#253
  // Define these so class becomes non POD for const initalizaiton
  SerializableHandle() {}
  ~SerializableHandle() {}

  inline InputSystem::KeyBindMap fromJSON(const Json::Value& v) const {
    InputSystem::KeyBindMap map;
    for ( auto& val : v.getMemberNames() ) {
      KeyInput b = Serializable::fromJSON<KeyInput>(v[val]);

      // Handle a bit of the fragile string event system here.
      if ( b.userInputType == KeyboardInput ) {
        b.eventType = sf::Event::KeyPressed;
        map["+" + val] = b;

        b.eventType = sf::Event::KeyReleased;
        map["-" + val] = b;
      } else if ( b.userInputType == MouseInput ) {
        b.eventType = sf::Event::MouseButtonPressed;
        map["+" + val] = b;

        b.eventType = sf::Event::MouseButtonReleased;
        map["-" + val] = b;
      } else {
        // Do nothing
        (void)b;
      }
    }
    return map;
  }

  inline Json::Value toJSON(const InputSystem::KeyBindMap& map) const {
    Json::Value v;
    for ( auto& pair : map ) {
      // Handle a bit of the fragile string event system here.
      std::regex rek("^[+-](.*)");
      std::smatch match;
      if ( std::regex_search(pair.first, match, rek) && match.size() > 1 ) {
        v[match[1].str()] = Serializable::toJSON<KeyInput>(pair.second);
      } else {
        // Never should happen.
        assert(!"Something has gone horribly wrong");
      }
    }
    return v;
  }
};

#include <Box2D/Box2D.h>

template<>
class SerializableHandle<b2BodyType> : public SerializableHandleEnum<b2BodyType> {
public:
  // Workaround: DR253: http://www.open-std.org/jtc1/sc22/wg21/docs/cwg_active.html#253
  // Define these so class becomes non POD for const initalizaiton
  SerializableHandle() : SerializableHandleEnum<b2BodyType>({
    {"DYNAMIC", b2BodyType::b2_dynamicBody},
    {"STATIC", b2BodyType::b2_staticBody},
    {"KINEMATIC", b2BodyType::b2_kinematicBody}
  }) {
  }
  ~SerializableHandle() {}
};

template<>
class SerializableHandle<CollisionCategory> : public SerializableHandleEnum<CollisionCategory> {
public:
  // Workaround: DR253: http://www.open-std.org/jtc1/sc22/wg21/docs/cwg_active.html#253
  // Define these so class becomes non POD for const initalizaiton
  SerializableHandle() : SerializableHandleEnum<CollisionCategory>({
    { "CATEGORY_1", CollisionCategory::CATEGORY_1 },
    { "CATEGORY_2", CollisionCategory::CATEGORY_2 },
    { "CATEGORY_3", CollisionCategory::CATEGORY_3 },
    { "CATEGORY_4", CollisionCategory::CATEGORY_4 },
    { "CATEGORY_5", CollisionCategory::CATEGORY_5 },
    { "CATEGORY_6", CollisionCategory::CATEGORY_6 },
    { "CATEGORY_7", CollisionCategory::CATEGORY_7 },
    { "CATEGORY_8", CollisionCategory::CATEGORY_8 },
    { "CATEGORY_9", CollisionCategory::CATEGORY_9 },
    { "CATEGORY_10", CollisionCategory::CATEGORY_10 },
    { "CATEGORY_11", CollisionCategory::CATEGORY_11 },
    { "CATEGORY_12", CollisionCategory::CATEGORY_12 },
    { "CATEGORY_13", CollisionCategory::CATEGORY_13 },
    { "CATEGORY_14", CollisionCategory::CATEGORY_14 },
    { "CATEGORY_15", CollisionCategory::CATEGORY_15 },
    { "CATEGORY_16", CollisionCategory::CATEGORY_16 }
  }) {
  }
  ~SerializableHandle() {}
};


template<>
class SerializableHandle<CollisionCategoryBitset> {
public:
  // Workaround: DR253: http://www.open-std.org/jtc1/sc22/wg21/docs/cwg_active.html#253
  // Define these so class becomes non POD for const initalizaiton
  SerializableHandle() {}
  ~SerializableHandle() {}

  inline Json::Value toJSON(const CollisionCategoryBitset& s) const {
    Json::Value o;
    o = s.to_string();
    return o;
  }

  inline CollisionCategoryBitset fromJSON(const Json::Value& v) const {
    return CollisionCategoryBitset(std::string(v.asString()));
  }

  void initPy(py::class_<CollisionCategoryBitset>&& py) const {
    py.add_property("bits",
                    &CollisionCategoryBitset::get_bits,
                    &CollisionCategoryBitset::set_bits);
  }
};

template<>
class SerializableHandle<sf::Color> {
public:
  // Workaround: DR253: http://www.open-std.org/jtc1/sc22/wg21/docs/cwg_active.html#253
  // Define these so class becomes non POD for const initalizaiton
  SerializableHandle() {}
  ~SerializableHandle() {}

  inline Json::Value toJSON(const sf::Color& c) const {
    Json::Value o;
    o[0] = c.r;
    o[1] = c.g;
    o[2] = c.b;
    o[3] = c.a;
    return o;
  }

  inline sf::Color fromJSON(const Json::Value& v) const {
    auto r = v[0].asInt();
    auto g = v[1].asInt();
    auto b = v[2].asInt();
    auto a = v[3].asInt();
    // Default alpha
    a = (a <= 0) ? 255 : a;
    return sf::Color(r,g,b,a);
  }

  void initPy(py::class_<sf::Color>&& py) const {
    //TODO(SMA): Add setter/getter
  }
};