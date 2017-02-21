// Copyright 2016 Bablawn3d5

#include <Farquaad/Core/EntitySerializer.h>
#include <Farquaad/Components.hpp>
#include <Meta.h>
#include <Farquaad/Core/MetaRegister.hpp>
#include <entityx/python/PythonScript.hpp>

using PythonScript = entityx::python::PythonScript;

std::string EntitySerializer::toString() const {
    return cs.toString();
}

void EntitySerializer::Load(ex::Entity entity) const {
    //auto& map = SeralizeableComponentMap::get();
    //assert(map.nameToLoadFunc.size() != 0);
    //for ( auto& key : map.nameToLoadFunc ) {
    //    key.second(this->cs, entity);
    //}

  // FIXME(SMA) : Hard code workaround. Fix me.
  ComponentSerializer::LoadComponentToEntity<Body>(this->cs, entity);
  ComponentSerializer::LoadComponentToEntity<Physics>(this->cs, entity);
  ComponentSerializer::LoadComponentToEntity<Stats>(this->cs, entity);
  ComponentSerializer::LoadComponentToEntity<InputResponder>(this->cs, entity);
  ComponentSerializer::LoadComponentToEntity<PythonScript>(this->cs, entity);
  ComponentSerializer::LoadComponentToEntity<Renderable>(this->cs, entity);
}

// HACK(SMA) : Ugly assigment hack for the Save function below. Really needs to be fixed.
void AssignIfNotNull(Json::Value&& l, const std::string& name, Json::Value& r) {
  if( r == Json::nullValue ) return;
  l[name] = r;
}

Json::Value EntitySerializer::Save(ex::Entity entity) const {
    Json::Value v;
    //auto& map = SeralizeableComponentMap::get();
    //assert(map.nameToSaveFunc.size() != 0);
    //for ( auto& key : map.nameToSaveFunc ) {
    //    auto json = key.second(this->cs, entity);
    //    if ( json == Json::nullValue ) continue;
    //    v[key.first] = json[key.first];
    //}

    //FIXME(SMA) : Hard code workaround. Fix me.
    AssignIfNotNull(std::move(v), meta::getName<Body>(),
      ComponentSerializer::SaveEntityComponent<Body>(this->cs, entity)[meta::getName<Body>()]);
    AssignIfNotNull(std::move(v), meta::getName<Physics>(),
      ComponentSerializer::SaveEntityComponent<Physics>(this->cs, entity)[meta::getName<Physics>()]);
    AssignIfNotNull(std::move(v), meta::getName<Stats>(),
      ComponentSerializer::SaveEntityComponent<Stats>(this->cs, entity)[meta::getName<Stats>()]);
    AssignIfNotNull(std::move(v), meta::getName<InputResponder>(),
      ComponentSerializer::SaveEntityComponent<InputResponder>(this->cs, entity)[meta::getName<InputResponder>()]);
    AssignIfNotNull(std::move(v), meta::getName<Renderable>(),
                    ComponentSerializer::SaveEntityComponent<Renderable>(this->cs, entity)[meta::getName<Renderable>()]);
    AssignIfNotNull(std::move(v), meta::getName<PythonScript>(),
      ComponentSerializer::SaveEntityComponent<PythonScript>(this->cs, entity)[meta::getName<PythonScript>()]);
    return v;
}
