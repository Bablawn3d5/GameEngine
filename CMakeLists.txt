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

# Disable examples/third party testing by default
option(BUILD_SHARED_LIBS "Link SFML as shared or not" OFF)
if(BUILD_SHARED_LIBS)
  set(ENTITYX_BUILD_SHARED true CACHE BOOL "Build shared libraries?")
  set(THOR_SHARED_LIBS ON CACHE PATH "Thor top-level directory")
 set(ENTITYX_PYTHON_BUILD_SHARED ON CACHE BOOL "Enable building shared entityx python libraries" )
else()
  set(ENTITYX_BUILD_SHARED false CACHE BOOL "Build shared libraries?")
  set(THOR_SHARED_LIBS OFF CACHE PATH "Thor top-level directory")
  set(ENTITYX_PYTHON_BUILD_SHARED OFF CACHE BOOL "Enable building shared entityx python libraries" )
endif()
option(SFML_OPENGL_ES "Build using ES opengl for non-desktop?" OFF)
option(BOX2D_BUILD_EXAMPLES "Build Box2D examples" OFF)
option(BOX2D_INSTALL "Install Box2D libs, includes, and CMake scripts" OFF)
option(THOR_SFML_IS_BUILT "Set to automatically link SFML includes, as you build the enitre SFML source as part of your"  ON)
option(ENTITYX_BUILD_TESTING "Enable building of tests." OFF)
option(ENTITYX_PYTHON_BUILD_TESTING "Enable building shared entityx python tests" OFF)

enable_testing()

# TODO(SMA): You'll have to install python on the system yourself for now
# till we compile CPython.
find_package(PythonLibs 2.7 REQUIRED)
message(status "** Python Include: ${PYTHON_INCLUDE_DIRS}")
message(status "** Python Libraries: ${PYTHON_LIBRARIES}")
if(NOT PYTHONLIBS_FOUND)
    set(PYTHON_ROOT "" CACHE PATH "PYTHON top-level directory")
    message("---> Python 2.7 directory not found. Set PYTHON_ROOT to Pyons's top-level path (containing \"include\" and \"lib\" directories).\n")
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

# Set libdirs to output directory because cmake is very confused when we add_subdirectory a 
# project.
message ("WTF? ${CMAKE_BINARY_DIR}/lib/${CMAKE_CFG_INTDIR}/")
link_directories(${CMAKE_BINARY_DIR}/lib/)

# TODO(SMA) : Use ExternalProject_Add to conditionally build external libs.
add_subdirectory(${GameEngine_SOURCE_DIR}/extinclude/entityx-repo)

# Override entityx include with ours.
configure_file(
    ${GameEngine_SOURCE_DIR}/extinclude/entityx/config.h.in
    ${GameEngine_SOURCE_DIR}/extinclude/entityx-repo/entityx/config.h
)

# Add entityx python with a bit of tweaking.
set(ENTITYX_FOUND true)
set(ENTITYX_LIBRARY entityx)
set(ENTITYX_LIBRARY_RELEASE entityx)
set(ENTITYX_LIBRARY_DEBUG entityx-d)
set(ENTITYX_INCLUDE_DIR ${EntityX_SOURCE_DIR})
set(ENTITYX_LIBRARIES optimized ${ENTITYX_LIBRARY_RELEASE} debug ${ENTITYX_LIBRARY_DEBUG})
add_subdirectory(${GameEngine_SOURCE_DIR}/extinclude/entityx_pybind11-repo)

set(ENTITYX_PYBIND11_LIBRARY entityx_python)
set(ENTITYX_PYBIND11_LIBRARY_RELEASE entityx_python)
set(ENTITYX_PYBIND11_LIBRARY_DEBUG entityx_python-d)
set(ENTITYX_PYBIND11_INCLUDE_DIR ${EntityX_PYBIND11_SOURCE_DIR})
set(ENTITYX_PYBIND11_LIBRARIES optimized ${ENTITYX_PYBIND11_LIBRARY_RELEASE} debug ${ENTITYX_PYBIND11_LIBRARY_DEBUG})
message(status "** ENTITYX_PYBIND11 Include: ${ENTITYX_PYBIND11_INCLUDE_DIR}")
message(status "** ENTITYX_PYBIND11 Libs: ${ENTITYX_PYBIND11_LIBRARIES}")

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
