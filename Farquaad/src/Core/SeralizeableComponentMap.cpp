// Copyright 2015 Bablawn3d5
#include <Farquaad/Core/SeralizeableComponentMap.h>

void SeralizeableComponentMap::RegisterLoad(const std::string & name, LoadFunciton serlize_func) {
    assert(isRegistered(name) == false);
    nameToLoadFunc[name] = serlize_func;
}

void SeralizeableComponentMap::RegisterSave(const std::string & name, SaveFunction serlize_func) {
}
