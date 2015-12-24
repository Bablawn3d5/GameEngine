cmake_minimum_required(VERSION 3.1)

project(FarquaadExamples)

# Tell CMake to build a Farquaad executable
add_executable(FarquaadExamples main.cpp)
include_directories(${Farquaad_SOURCE_DIR}/include)
include_directories(${jsoncpp_SOURCE_DIR}/include)
include_directories(${EntityX_SOURCE_DIR})
include_directories(${SFML_SOURCE_DIR}/include)
include_directories(${Box2D_SOURCE_DIR})


# Copies shared DLLs to OutputDir by setting build output to binary dir
set_target_properties(sfml-window PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${FarquaadExamples_BINARY_DIR}")
set_target_properties(sfml-graphics PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${FarquaadExamples_BINARY_DIR}")
set_target_properties(sfml-system PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${FarquaadExamples_BINARY_DIR}")
set_target_properties(entityx_shared PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${FarquaadExamples_BINARY_DIR}")
set_target_properties(Farquaad_shared PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${FarquaadExamples_BINARY_DIR}")
# JSONCPP for god knows reasons sets this global.
if(BUILD_SHARED_LIBS)
	set_target_properties(jsoncpp_lib PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${FarquaadExamples_BINARY_DIR}")
endif()

if(BOX2D_BUILD_SHARED)
    set_target_properties(Box2D_shared PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${FarquaadExamples_BINARY_DIR}")
endif()

target_link_libraries(FarquaadExamples Farquaad_shared)

# Set installation directory to incl Packages
install(TARGETS FarquaadExamples DESTINATION bin)
if(MSVC)
    if (CMAKE_BUILD_TYPE STREQUAL 'Debug')
        set(CMAKE_INSTALL_DEBUG_LIBRARIES 1)
    endif()
    include(InstallRequiredSystemLibraries)
endif()
