cmake_minimum_required(VERSION 3.1)
set(BABLAWN_GAMEENGINE_MAJOR_VERSION 0)
set(BABLAWN_GAMEENGINE_MINOR_VERSION 0)
set(BABLAWN_GAMEENGINE_PATCH_VERSION 1)
set(BABLAWN_GAMEENGINE_VERSION ${BABLAWN_GAMEENGINE_MAJOR_VERSION}.
${BABLAWN_GAMEENGINE_MINOR_VERSION}.
${BABLAWN_GAMEENGINE_PATCH_VERSION})

project(GameEngine)

#Disable examples by default
option(BOX2D_BUILD_EXAMPLES "Build Box2D examples" OFF)
option(BOX2D_INSTALL "Install Box2D libs, includes, and CMake scripts" OFF)

add_subdirectory(${GameEngine_SOURCE_DIR}/extinclude/entityx-repo)
add_subdirectory(${GameEngine_SOURCE_DIR}/extinclude/Box2D-repo/Box2D)
add_subdirectory(${GameEngine_SOURCE_DIR}/extinclude/SFML-Repo)
add_subdirectory(${GameEngine_SOURCE_DIR}/Farquaad)
