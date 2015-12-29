// Copyright 2015 Bablawn3d5

// Extensions to thor::ResoruceLoader
#pragma once

#include <Thor/Resources.hpp>
#include <json/json.h>
#include <fstream>

namespace Resources {
thor::ResourceLoader<Json::Value> loadJSON(const char* filename) {
    auto& loadingFunc = [=]() {
        std::ifstream fileStream(filename);
        Json::Value v;
        fileStream >> v;
        return std::unique_ptr<Json::Value>(new Json::Value(v));
    };
    return thor::ResourceLoader<Json::Value>(loadingFunc, filename);
}
}
