// Copyright 2016 Bablawn3d5

#pragma once

#include <Meta.h>
#include <Farquaad/Components.hpp>
#include <entityx/python/PythonScript.hpp>

using PythonScript = entityx::python::PythonScript;

namespace meta
{

template <>
constexpr auto registerName<Body>() {
  return "body";
}

template <>
inline auto registerMembers<Body>() {
    return members(
        member("position", &Body::position),
        member("direction", &Body::direction)
    );
}

template <>
constexpr auto registerName<Sound>() {
  return "sound";
}

template <>
inline auto registerMembers<Sound>() {
  return members(
    member("name", &Sound::name),
    member("pitch", &Sound::pitch),
    member("volume", &Sound::volume),
    member("start_delay", &Sound::start_delay),
    member("play_count", &Sound::play_count)
  );
}

template <>
constexpr auto registerName<PythonScript>() {
  return "pythons";
}

template <>
inline auto registerMembers<PythonScript>() {
  return members(
    member("modulename", &PythonScript::module),
    member("class", &PythonScript::cls)
    );
}

template <>
constexpr auto registerName<Destroyed>() {
  return "destroyed";
}

template <>
inline auto registerMembers<Destroyed>() {
  return members(
    member("timer", &Destroyed::deathTimer)
  );
}


template <>
constexpr auto registerName<Stats>() {
  return "stats";
}

template <>
inline auto registerMembers<Stats>() {
    return members(
        member("curHP", &Stats::currentHp),
        member("maxHP", &Stats::maxHp),
        member("speed", &Stats::speed),
        member("god", &Stats::godmode)
    );
}

template <>
constexpr auto registerName<Physics>() {
  return "physics";
}

template <>
inline auto registerMembers<Physics>() {
    return members(
      member("bodyType", &Physics::bodyType),
      member("size", &Physics::size),
      member("dirty", &Physics::isDirty),
      member("category", &Physics::collisionCategory),
      member("mask", &Physics::collisionMask),
      member("group", &Physics::collisionGroup),
      member("currentCollisions", &Physics::collidingWithSet),
      member("collisionCount", &Physics::collisionCount),
      member("isColliding", &Physics::isColliding),
      member("isSensor", &Physics::isSensor),
      member("isBullet", &Physics::isBullet)
    );
}


template <>
constexpr auto registerName<InputResponder>() {
  return "inputresponder";
}

template <>
inline auto registerMembers<InputResponder>() {
  return members(
    member("responds", &InputResponder::responds),
    member("mousePos", &InputResponder::mousePos),
    member("mouseGamePos", &InputResponder::mouseGamePos)
    );
}

template <>
constexpr auto registerName<Renderable>() {
  return "renderable";
}

template <>
inline auto registerMembers<Renderable>() {
  return members(
    member("texture", &Renderable::texture_name),
    member("currentAnim", &Renderable::current_animation),
    member("frame", &Renderable::cur_frame),
    member("frameTime", &Renderable::cur_frame_time),
    member("r", &Renderable::r),
    member("g", &Renderable::g),
    member("b", &Renderable::b),
    member("a", &Renderable::a),
    member("scale", &Renderable::scale),
    member("offset", &Renderable::offset),
    member("rotation", &Renderable::rotation),
    member("layer", &Renderable::layer),
    member("font", &Renderable::font_name),
    member("fontString", &Renderable::font_string),
    member("fontSize", &Renderable::font_size),
    member("dirty", &Renderable::isDirty)
  );
}
} // namespace meta
