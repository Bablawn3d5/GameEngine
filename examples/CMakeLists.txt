cmake_minimum_required(VERSION 3.1)

project(FarquaadExamples)

# Default compiler args
if (CMAKE_CXX_COMPILER_ID MATCHES "(GNU|.*Clang)")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pedantic -Werror -Wall -Wextra -Wno-unused-parameter -Wno-error=unused-variable -Wno-error=sign-compare -std=c++11")
     if (CMAKE_CXX_COMPILER_ID MATCHES "(.*Clang)")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Qunused-arguments")
    endif()
    set(CMAKE_CXX_FLAGS_DEBUG "-O0 -g")
    set(CMAKE_CXX_FLAGS_MINSIZEREL "-g -Os -DNDEBUG")
    set(CMAKE_CXX_FLAGS_RELEASE "-g -O2 -DNDEBUG")
    set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-O2 -g")
elseif(CMAKE_CXX_COMPILER_ID STREQUAL 'MSVC')
    # /Zi - Produces a program database (PDB) that contains type information and symbolic debugging information for use with the debugger.
    # /FS - Allows multiple cl.exe processes to write to the same .pdb file
    # /DEBUG - Enable debug during linking
    # /Od - Disables optimization
    set(CMAKE_CXX_FLAGS_DEBUG "/Zi /FS /DEBUG /Od /MDd")
    # /Ox - Full optimization
    set(CMAKE_CXX_FLAGS_RELEASE "/Ox -DNDEBUG")
    set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "/Ox /Zi /FS /DEBUG")
endif()

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
