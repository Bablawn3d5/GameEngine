cmake_minimum_required(VERSION 3.1)

project(FarquaadExamples)

# Tell CMake to build a Farquaad executable
add_executable(FarquaadExamples main.cpp)
include_directories(${Farquaad_SOURCE_DIR}/include)
include_directories(${jsoncpp_SOURCE_DIR}/include)
include_directories(${EntityX_SOURCE_DIR})
include_directories(${SFML_SOURCE_DIR}/include)
include_directories(${Box2D_SOURCE_DIR})

target_link_libraries(FarquaadExamples Farquaad)

# Set installation directory to incl Packages
install(TARGETS FarquaadExamples DESTINATION bin)
if(MSVC)
    if (CMAKE_BUILD_TYPE STREQUAL 'Debug')
        set(CMAKE_INSTALL_DEBUG_LIBRARIES 1)
    endif()
    include(InstallRequiredSystemLibraries)
endif()
