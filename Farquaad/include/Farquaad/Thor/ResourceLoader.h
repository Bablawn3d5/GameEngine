// Copyright 2015-2016 Bablawn3d5

// Extensions to thor::ResoruceLoader
#pragma once

#include <Thor/Resources.hpp>
#include <json/json.h>

namespace Resources {
thor::ResourceLoader<Json::Value> loadJSON(const std::string& filename);
}
