# Bablawn3d5 GameEngine - A bootstrap project for quick C++/Python Jams

Bablawn3d5 - GameEngine

Linux/Mac: [![Build Status](https://travis-ci.org/Bablawn3d5/GameEngine.svg?branch=master)](https://travis-ci.org/Bablawn3d5/GameEngine) 

Windows: [![Build status](https://ci.appveyor.com/api/projects/status/4kyg48lhpudisk6f/branch/master?svg=true)](https://ci.appveyor.com/project/PHEN-/gameengine/branch/master)

Boostraps serveral C++ libraries the Bablawn uses into a covinent "engine". Build is quite bulky, as we intend to keep all major package requirements part of the repo for easy setup.

[EntityX](https://github.com/alecthomas/entityx)

[Box2D](https://github.com/erincatto/Box2D)

[SFML](https://github.com/SFML/SFML)

[jsoncpp](https://github.com/open-source-parsers/jsoncpp.git)

[Catch](https://github.com/philsquared/Catch)

[Thor](https://github.com/Bromeon/Thor)

[MetaStuff](https://github.com/EliasD/MetaStuff)

[ImGui](https://github.com/ocornut/imgui)

[ImGui-SFML](https://github.com/EliasD/imgui-sfml)

## Building

The engine builds on Windows, Linux and MacOSX. Built with:  **Visual Studio 2015 - Update 3**, **gcc-4.9**, and **clang-3.8**. 

You will need to install **Python 2.7** and **Boost.Python 1.59** on your system to build from source.

Build scripts are built using **CMake 3.1.0** or Higher.

There's a number of prerequisite libraries needed on Linux64 and MacOSX, see [.travis.yml](.travis.yml) for the list of packages.

```
git clone https://github.com/Bablawn3d5/GameEngine.git
git submodule update --init --recursive
mkdir Build && cd Build
cmake ..
make
```

## Distribution

We use cpack to generate our distributions.

```
cd Build
cmake -DCMAKE_INSTALL_PREFIX=`pwd`/package
cmake ..

# This will create a GameEngine-<version>-<commit>-<os>.zip and .tar file
# under <cwd>/package
cpack
```

##  Serializer

The serializer comes bundled with the engine, intergartes bindings for jsoncpp and python:


```
  Body b{10,10};
  Json::Value v = Serializable::toJSON(b);
  // Will output {"position" : [10,10]}
  std::cout << v;
```

Going from JSON to Objects:

```
  Json::Value v;
  std::stringstream s;
  s << "{\"position\":[10,-10]}";
  s >> v;

  // Body will be initalized with position = (10, -10);
  Body b =  Serializable::fromJSON<Body>(v);
```


Going from Python to Objects:

```
BOOST_PYTHON_MODULE(_entityx_components) {
  Serializable::initPy(py::class_<Body>("Body", py::init<>()));
}
```

Extending the serializer to new types is as easy as defining a SerializableHandle, and mapping members to a Meta header:

```
// Some new component
struct Position {
    sf::Vector2f position;
    int someInt;
    bool someBool;
};

// This meta definition will auto-generate Serializable::fromJSON<Position>(&p) and 
// Serializable::toJSON<Position>(&p) 
namespace meta
{
template <>
constexpr auto registerName<Position>() {
  return "body";
}

template <>
inline auto registerMembers<Position>() {
    return members(
        member("position", &Position::position),
        member("someInt", &Position::someInt),
        member("someBool", &Position::someBool)
        );
    }
}

```

## Copying

The Bablawn3d5 GameEngine itself is licensed and distributed under zlib license.

Copyright 2015-2016 Bablawn3d5, all rights reserved.

See `license.txt` for all related third party and open source licenses in this repository.
