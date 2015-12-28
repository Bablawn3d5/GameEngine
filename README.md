# GameEngine [![Build Status](https://travis-ci.org/Bablawn3d5/GameEngine.svg?branch=master)](https://travis-ci.org/Bablawn3d5/GameEngine) [![Build status](https://ci.appveyor.com/api/projects/status/4kyg48lhpudisk6f/branch/master?svg=true)](https://ci.appveyor.com/project/PHEN-/gameengine/branch/master)
Bablawn3d5 Game Engine

Binds serveral C++ libraries the Bablawn uses into a covinent "engine". Build is quite bulky, as we intend to keep all major package requirements part of the repo. 

[EntityX](https://github.com/alecthomas/entityx)

[Box2D](https://github.com/erincatto/Box2D)

[SFML](https://github.com/SFML/SFML)

## Building

The engine builds on Win64 **Visual Studio 2015**, **gcc-4.9**, and **clang-3.5**. Anything earlier may encounter compiler issues with the C++11 language.

Build scripts are built using CMake 3.1.0 or Higher.

There's a number of prequisit libraries needed on Linux64 and MacOSX, see `.travis.yml` for the list of packages.

```
mkdir Build
cd Build
cmake ..
make
```

## Distribution

We primiarly use cpack to generate our distributions.

```
cd Build
cmake -DCMAKE_INSTALL_PREFIX=`pwd`/package
cmake ..

# This will create a GameEngine-<version>-<commit>-<os>.zip and .tar file
# under <cwd>/package
cpack 
```

##  Serializer 

The serializer comes bundled with the enigne, intergartes bindings for jsoncpp:


```
  Body b{10,10};
  Json::Value v = Serializable::toJSON(b);
  // Will output {"position" : [10,10]}
  std::cout << v;
```

Going from JSON to Objects is simple too:

```
  Json::Value v;
  std::stringstream s;
  s << "{\"position\":[10,-10]}";
  s >> v;

  // Body will be initalized with position = (10, -10);
  Body b =  Serializable::fromJSON<Body>(v);
```

Extending the serializer to new types is as easy as defining a SerializableHandle, and mapping members to a MemberMap:

```
// Some new component
struct Position {
    sf::Vector2f position;
    int someInt;
    bool someBool;
};

// Defined in some header file:
template<>
class SerializableHandle<Position> : public SerializeFromRegistry<Position> {
public:
    SerializableHandle() : SerializeFromRegistry<Position>(this->GenerateMap()) {
    }

    inline const SerializeFromRegistry<Position>::MemberMap SerializableHandle<Position>::GenerateMap() {
        SerializeFromRegistry<Position>::MemberMap map;
        AddMember(map, "position", &Position::position);
        AddMember(map, "someInt", &Position::someInt);
        AddMember(map, "someBool", &Position::someBool);
        return map;
    }
}

```

## Copying

The Bablawn3d5 GameEngine itself is licensed and distributed under zlib license. 

See `license.txt` for all related third party and open source licenses in this repository.
