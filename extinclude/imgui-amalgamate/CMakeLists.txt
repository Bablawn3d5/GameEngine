cmake_minimum_required(VERSION 3.1)

project(imgui-amalgamate)

include_directories(${PROJECT_SOURCE_DIR}/imgui-repo)
include_directories(${PROJECT_SOURCE_DIR}/imgui-sfml-repo)
set(IMGUI_AMALGAMATE_INCLUDE_DIRS
    ${PROJECT_SOURCE_DIR}/imgui-repo
    ${PROJECT_SOURCE_DIR}/imgui-sfml-repo
    PARENT_SCOPE
    )
set(IMGUI_AMALGAMATE_SOURCES
    ${PROJECT_SOURCE_DIR}/imgui-repo/imgui.cpp
    ${PROJECT_SOURCE_DIR}/imgui-repo/imgui_draw.cpp
    ${PROJECT_SOURCE_DIR}/imgui-sfml-repo/imgui-SFML.cpp
    PARENT_SCOPE)
