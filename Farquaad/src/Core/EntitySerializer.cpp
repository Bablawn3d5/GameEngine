// Copyright 2016 Bablawn3d5
#include <Farquaad/Core/EntitySerializer.h>
#include <Farquaad/Core/SeralizeableComponentMap.h>

std::string EntitySerializer::toString() const {
    return cs.toString();
}

void EntitySerializer::Load(ex::Entity entity) const {
    auto& map = SeralizeableComponentMap::get();
    for ( auto& key : map.nameToLoadFunc ) {
        key.second(this->cs, entity);
    }
}

Json::Value EntitySerializer::Save(ex::Entity entity) const {
    Json::Value v;
    auto& map = SeralizeableComponentMap::get();
    for ( auto& key : map.nameToSaveFunc ) {
        auto json = key.second(this->cs, entity);
        v[key.first] = json[key.first];
    }

    return v;
}
