cmake_minimum_required(VERSION 3.1)
set(BABLAWN_FARQUAAD_MAJOR_VERSION 0)
set(BABLAWN_FARQUAAD_MINOR_VERSION 0)
set(BABLAWN_FARQUAAD_PATCH_VERSION 1)
set(BABLAWN_FARQUAAD_VERSION ${BABLAWN_FARQUAAD_MAJOR_VERSION}.
${BABLAWN_FARQUAAD_MINOR_VERSION}.
${BABLAWN_FARQUAAD_PATCH_VERSION})

project(Farquaad)

# Set to new behaviour, we don't mean to expand quoted variables here.
cmake_policy(SET CMP0054 NEW)

# Default compiler args
if ("${CMAKE_CXX_COMPILER_ID}" MATCHES "(GNU|.*Clang)")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pedantic -Werror -Wall -Wextra -Wno-unused-parameter -Wno-error=unused-variable -Wno-error=sign-compare -std=c++11")
	set(CMAKE_CXX_FLAGS_DEBUG "-O0 -g")
	set(CMAKE_CXX_FLAGS_MINSIZEREL "-g -Os -DNDEBUG")
	set(CMAKE_CXX_FLAGS_RELEASE "-g -O2 -DNDEBUG")
	set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-O2 -g")
elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
	# /Zi - Produces a program database (PDB) that contains type information and symbolic debugging information for use with the debugger.
	# /FS - Allows multiple cl.exe processes to write to the same .pdb file
	# /DEBUG - Enable debug during linking
	# /Od - Disables optimization
	set(CMAKE_CXX_FLAGS_DEBUG "/Zi /FS /DEBUG /Od /MDd")
	# /Ox - Full optimization
	set(CMAKE_CXX_FLAGS_RELEASE "/Ox -DNDEBUG")
	set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "/Ox /Zi /FS /DEBUG")
endif()

# Library installation directory
if(NOT DEFINED CMAKE_INSTALL_LIBDIR)
    set(CMAKE_INSTALL_LIBDIR lib)
endif(NOT DEFINED CMAKE_INSTALL_LIBDIR)
set(libdir ${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_LIBDIR})

# TODO(SMA) : Generate a cmake file under 'src' to auto-magic include Farquaad sources.
# Add the source files
set(SRC_DIR "src")
set(INC_DIR "include")

set(
    SOURCES
    ${SRC_DIR}/main.cpp
)

# Tell CMake to build a Farquaad executable
include_directories(${INC_DIR})
add_executable(Farquaad ${SOURCES})

# Include SFML
include_directories(${SFML_SOURCE_DIR}/include)
link_directories(${SFML_LIBRARY_DIR})
# Copies SFML outputs to our binary dir
set_target_properties(sfml-window PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${Farquaad_BINARY_DIR}")
set_target_properties(sfml-graphics PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${Farquaad_BINARY_DIR}")
set_target_properties(sfml-system PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${Farquaad_BINARY_DIR}")
target_link_libraries(Farquaad sfml-window sfml-graphics sfml-system)

# Copy outputs to our binary dir
include_directories(${entityx_SOURCE_DIR}/include)
link_directories(${entityx_LIBRARY_DIR})
set_target_properties(entityx PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${Farquaad_BINARY_DIR}")
set_target_properties(entityx_shared PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${Farquaad_BINARY_DIR}")
target_link_libraries(Farquaad entityx entityx_shared)
