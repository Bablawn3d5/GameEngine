cmake_minimum_required(VERSION 3.1)

project(GameEngine)

# Set everyone to output in the same spot
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_CONFIGURATION_TYPES "Debug;Release;RelWithDebInfo;" CACHE STRING "Limited configurations" FORCE)
mark_as_advanced(CMAKE_CONFIGURATION_TYPES)


# Set this to binary dir by default
if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
    set(CMAKE_INSTALL_PREFIX ${CMAKE_CURRENT_BINARY_DIR} CACHE
        PATH "FOO install prefix" FORCE)
endif(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)

# Get git hash
execute_process(
  COMMAND git log -1 --format=%h
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
  OUTPUT_VARIABLE GIT_COMMIT_HASH
  OUTPUT_STRIP_TRAILING_WHITESPACE
)

set(CPACK_PACKAGE_FILE_NAME "GameEngine-0.2.0-${GIT_COMMIT_HASH}-${CMAKE_SYSTEM_NAME}")

# Disable examples by default
option(BUILD_SHARED_LIBS "Link SFML as shared or not" OFF)
option(SFML_OPENGL_ES "Build using ES opengl for non-desktop?" OFF)
option(BOX2D_BUILD_EXAMPLES "Build Box2D examples" OFF)
option(BOX2D_INSTALL "Install Box2D libs, includes, and CMake scripts" OFF)
option(THOR_SFML_IS_BUILT "Set to automatically link SFML includes, as you build the enitre SFML source as part of your"  ON)

enable_testing()

# HACK(SMA): Your on your own kid-o for boost and python libs
find_package(PythonLibs 2.7 REQUIRED)
message(status "** Python Include: ${PYTHON_INCLUDE_DIRS}")
message(status "** Python Libraries: ${PYTHON_LIBRARIES}")
if(NOT PYTHONLIBS_FOUND)
    set(PYTHON_ROOT "" CACHE PATH "PYTHON top-level directory")
    message("---> Python 2.7 directory not found. Set PYTHON_ROOT to Pyons's top-level path (containing \"include\" and \"lib\" directories).\n")
endif()

SET(Boost_USE_STATIC_LIBS     ON)
SET(Boost_USE_MULTITHREADED    ON)
SET(Boost_USE_STATIC_RUNTIME     OFF)
find_package(Boost 1.59.0 COMPONENTS python filesystem system REQUIRED)
message(status "** Boost Include: ${Boost_INCLUDE_DIR}")
message(status "** Boost Dir: ${Boost_LIBRARY_DIRS}")
message(status "** Boost Libraries: ${Boost_LIBRARIES}")
if(NOT Boost_FOUND)
    set(BOOST_ROOT "" CACHE PATH "BOOST top-level directory")
    message("---> Boost 1.59 directory not found. Set BOOST_ROOT to Boost's top-level path (containing \"include\" and \"lib\" directories).\n")
else()
  # Assume we're good to go.
endif()

# Include Built Copy of Python27.dll
# Find Python DLLS
find_file(PYTHON_LIBRARY_DLL NAMES Python27.dll python2.7.dll)
SET(CMAKE_INSTALL_SYSTEM_RUNTIME_LIBS
       ${CMAKE_INSTALL_SYSTEM_RUNTIME_LIBS} "${PYTHON_LIBRARY_DLL}")

if(THOR_SFML_IS_BUILT)
    set(SFML_LIBRARIES "sfml-graphics" "sfml-window" "sfml-audio" "sfml-system")
    add_definitions(-DSFML_STATIC)
endif()

if(NOT BUILD_SHARED_LIBS)
    set(THOR_SHARED_LIBS OFF CACHE PATH "BOOST top-level directory")
endif()


#Fake finding SFML
# find_package(SFML 2 COMPONENTS graphics window system audio)
# message(status "** SFML Include: ${SFML_INCLUDE_DIR}")
# message(status "** SFML Dir: ${SFML_LIBRARY_DIRS}")
# message(status "** SFML Libraries: ${SFML_LIBRARIES}")
# if(SFML_FOUND)
# elseif(THOR_SFML_IS_BUILT)
#     set(SFML_LIBRARIES "sfml-graphics" "sfml-window" "sfml-audio" "sfml-system")
# else()
#     set(SFML_ROOT "" CACHE PATH "SFML top-level directory")
#     message("\n-> SFML directory not found. Set SFML_ROOT to SFML's top-level path (containing \"include\" and \"lib\" directories).")
#     message("-> Make sure the SFML libraries with the same configuration (Release/Debug, Static/Dynamic) exist.\n")
# endif()


# TODO(SMA) : Use ExternalProject_Add to conditionally build external libs.
add_subdirectory(${GameEngine_SOURCE_DIR}/extinclude/entityx-repo)
add_subdirectory(${GameEngine_SOURCE_DIR}/extinclude/Box2D-repo/Box2D)
add_subdirectory(${GameEngine_SOURCE_DIR}/extinclude/SFML-Repo)
# Lie to everyone that we've found SFML.
set(SFML_FOUND True)
set(SFML_INCLUDE_DIR ${SFML_SOURCE_DIR}/include)
add_subdirectory(${GameEngine_SOURCE_DIR}/extinclude/Thor)
add_subdirectory(${GameEngine_SOURCE_DIR}/extinclude/AsepriteToSFMLImage)
#add_subdirectory(${GameEngine_SOURCE_DIR}/extinclude/MetaStuff-repo)
#HACK(SMA) : Set metatstuff include dir
set(METASTUFF_INCLUDE_DIRS ${GameEngine_SOURCE_DIR}/extinclude/MetaStuff-repo/include)
add_subdirectory(${GameEngine_SOURCE_DIR}/extinclude/catch-amalgamate)
add_subdirectory(${GameEngine_SOURCE_DIR}/extinclude/jsoncpp-amalgamate)
add_subdirectory(${GameEngine_SOURCE_DIR}/extinclude/imgui-amalgamate)
add_subdirectory(${GameEngine_SOURCE_DIR}/extinclude/Python27Lib-Repo)

# Meat of our 'engine'
add_subdirectory(${GameEngine_SOURCE_DIR}/Farquaad)
add_subdirectory(${GameEngine_SOURCE_DIR}/examples)

# Set generator to ZIP/tarball for convenience
set(CPACK_GENERATOR "ZIP;TGZ" )
include(CPack)
