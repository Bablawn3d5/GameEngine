# Install script for directory: C:/Users/Stephen/OneDrive/Projects/github/GameEngine/extinclude/SFML-Repo/src/SFML

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "C:/Program Files (x86)/SFML")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Release")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for each subdirectory.
  include("C:/Users/Stephen/OneDrive/Projects/github/GameEngine/extinclude/SFML/src/SFML/System/cmake_install.cmake")
  include("C:/Users/Stephen/OneDrive/Projects/github/GameEngine/extinclude/SFML/src/SFML/Main/cmake_install.cmake")
  include("C:/Users/Stephen/OneDrive/Projects/github/GameEngine/extinclude/SFML/src/SFML/Window/cmake_install.cmake")
  include("C:/Users/Stephen/OneDrive/Projects/github/GameEngine/extinclude/SFML/src/SFML/Network/cmake_install.cmake")
  include("C:/Users/Stephen/OneDrive/Projects/github/GameEngine/extinclude/SFML/src/SFML/Graphics/cmake_install.cmake")
  include("C:/Users/Stephen/OneDrive/Projects/github/GameEngine/extinclude/SFML/src/SFML/Audio/cmake_install.cmake")

endif()

