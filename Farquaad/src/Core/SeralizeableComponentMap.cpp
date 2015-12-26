// Copyright 2015 Bablawn3d5
#include <Farquaad/Core/SeralizeableComponentMap.h>

void SeralizeableComponentMap::Register(const std::string & name, AssignentFunction serlize_func) {
    assert(isRegistered(name) == false);
    nameToAssignor[name] = serlize_func;
}

bool SeralizeableComponentMap::isRegistered(const std::string & name) {
    auto it = nameToAssignor.find(name);
    if ( it != nameToAssignor.end() ) {
        return 1;
    }
    return 0;
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
