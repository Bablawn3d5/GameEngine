// Copyright 2015 Bablawn3d5

// Extensions to thor::ResoruceLoader
#pragma once

#include <Thor/Resources.hpp>
#include <json/json.h>
#include <fstream>
#include <string>
#include <iostream>



namespace Resources {
thor::ResourceLoader<Json::Value> loadJSON(const std::string& filename) {
    const auto& loadingFunc = [=]() {
        std::ifstream fileStream(filename);
        Json::Value v;
        try {
            fileStream >> v;
        }
        catch ( Json::RuntimeError e ) {
            std::cerr << "Something went wrong loading '" << filename << "' :" << std::endl
                << e.what() << std::endl;
        }
        return std::unique_ptr<Json::Value>(new Json::Value(v));
    };
    return thor::ResourceLoader<Json::Value>(loadingFunc, filename);
}
}
