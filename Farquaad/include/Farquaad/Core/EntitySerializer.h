// Copyright 2016 Bablawn3d5
#pragma once

#include <Farquaad/Serialization.hpp>
#include <entityx/entityx.h>
#include <json/json.h>
#include <string>

namespace ex = entityx;

class EntitySerializer {
public:
    explicit EntitySerializer(Json::Value value = Json::nullValue) : cs(value) {}
    explicit EntitySerializer(ComponentSerializer cs) : cs(cs) {}
    ~EntitySerializer() {}

    std::string toString() const;
    void Load(ex::Entity entity) const;
    Json::Value Save(ex::Entity entity) const;
private:
    ComponentSerializer cs;
};
