// Copyright 2015 Bablawn3d5
#include <Farquaad/Core/SeralizeableComponentMap.h>

void SeralizeableComponentMap::Register(const std::string & name, AssignentFunction serlize_func) {
    nameToAssignor[name] = serlize_func;
}

bool SeralizeableComponentMap::isRegistered(const std::string & key) {
    try {
        nameToAssignor.at(key);
    }
    catch ( std::out_of_range  e ) {
        return 0;
    }
    return 1;
}

// TODO(SMA) : Throw a warning or something when requested string is
// not registered.
void SeralizeableComponentMap::Create(const std::string name,
                                      const ComponentSerializer & cs, ex::Entity & e) const {
    auto it = nameToAssignor.find(name);
    if ( it != nameToAssignor.end() ) {
        if ( it->second ) {
            it->second(cs, e);
        }
    }
}
