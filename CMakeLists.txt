cmake_minimum_required(VERSION 3.1)
set(BABLAWN_GAMEENGINE_MAJOR_VERSION 0)
set(BABLAWN_GAMEENGINE_MINOR_VERSION 0)
set(BABLAWN_GAMEENGINE_PATCH_VERSION 1)
set(BABLAWN_GAMEENGINE_VERSION ${BABLAWN_GAMEENGINE_MAJOR_VERSION}.
${BABLAWN_GAMEENGINE_MINOR_VERSION}.
${BABLAWN_GAMEENGINE_PATCH_VERSION})

project(GameEngine)

add_subdirectory(${GameEngine_SOURCE_DIR}/extinclude/entityx-repo)
add_subdirectory(${GameEngine_SOURCE_DIR}/extinclude/SFML-repo)
add_subdirectory(${GameEngine_SOURCE_DIR}/Farquaad)
