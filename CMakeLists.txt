cmake_minimum_required(VERSION 3.1)

project(GameEngine)

# Set everyone to output in the same spot
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})


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

set(CPACK_PACKAGE_FILE_NAME "GameEngine-0.0.1-${GIT_COMMIT_HASH}-${CMAKE_SYSTEM_NAME}")

#Disable examples by default
option(BOX2D_BUILD_EXAMPLES "Build Box2D examples" OFF)
option(BOX2D_INSTALL "Install Box2D libs, includes, and CMake scripts" OFF)

OPTION(JSONCPP_WITH_TESTS "Compile and (for jsoncpp_check) run JsonCpp test executables" OFF)
OPTION(JSONCPP_WITH_POST_BUILD_UNITTEST "Automatically run unit-tests as a post build step" OFF)
OPTION(JSONCPP_WITH_WARNING_AS_ERROR "Force compilation to fail if a warning occurs" OFF)
OPTION(JSONCPP_WITH_STRICT_ISO "Issue all the warnings demanded by strict ISO C and ISO C++" ON)
OPTION(JSONCPP_WITH_PKGCONFIG_SUPPORT "Generate and install .pc files" OFF)
OPTION(JSONCPP_WITH_CMAKE_PACKAGE "Generate and install cmake package files" OFF)
OPTION(BUILD_SHARED_LIBS "Build jsoncpp_lib as a shared library." ON)
OPTION(BUILD_STATIC_LIBS "Build jsoncpp_lib static library." ON)

enable_testing()
add_subdirectory(${GameEngine_SOURCE_DIR}/extinclude/entityx-repo)
add_subdirectory(${GameEngine_SOURCE_DIR}/extinclude/Box2D-repo/Box2D)
add_subdirectory(${GameEngine_SOURCE_DIR}/extinclude/SFML-Repo)
add_subdirectory(${GameEngine_SOURCE_DIR}/extinclude/jsoncpp-repo)
add_subdirectory(${GameEngine_SOURCE_DIR}/Farquaad)
add_subdirectory(${GameEngine_SOURCE_DIR}/examples)

# Set generator to ZIP/tarball for convenience
set(CPACK_GENERATOR "ZIP;TGZ" )
include(CPack)
