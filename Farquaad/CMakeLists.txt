cmake_minimum_required(VERSION 3.1)
set(Farquaad_MAJOR_VERSION 0)
set(Farquaad_MINOR_VERSION 0)
set(Farquaad_PATCH_VERSION 1)
set(Farquaad_VERSION ${Farquaad_MAJOR_VERSION}.
${Farquaad_MINOR_VERSION}.
${Farquaad_PATCH_VERSION})

project(Farquaad)

# Default compiler args
if (CMAKE_CXX_COMPILER_ID MATCHES "(GNU|.*Clang)")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pedantic -Werror -Wall -Wextra -Wno-unused-parameter -Wno-error=unused-variable -Wno-error=sign-compare -std=c++11")
    set(CMAKE_CXX_FLAGS_DEBUG "-O0 -g")
    set(CMAKE_CXX_FLAGS_MINSIZEREL "-g -Os -DNDEBUG")
    set(CMAKE_CXX_FLAGS_RELEASE "-g -O2 -DNDEBUG")
    set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-O2 -g")
elseif(MAKE_CXX_COMPILER_ID STREQUAL 'MSVC')
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

set(INCROOT ${PROJECT_SOURCE_DIR}/include)
set(SRCROOT ${PROJECT_SOURCE_DIR}/src)

set(
    SOURCES
    ${SRCROOT}/main.cpp
)

# Link all directories
add_subdirectory(${SRCROOT})
include_directories(${INCROOT})

# Set source_group for Visual Studio organization
foreach(FILE ${INCLUDES} ${SOURCES})
    # Get the directory of the source file
    get_filename_component(PARENT_DIR "${FILE}" DIRECTORY)
    get_filename_component(PARENT_DIR "${PARENT_DIR}" ABSOLUTE)

    # Remove common directory prefix to make the group
    string(REPLACE "${CMAKE_CURRENT_SOURCE_DIR}" "" GROUP "${PARENT_DIR}")

    # Make sure we are using windows slashes
    string(REPLACE "/" "\\" GROUP "${GROUP}")

    # Remove redundant src\\
    string(REPLACE "src\\" "" GROUP "${GROUP}")
    string(REPLACE "\\src" "\\" GROUP "${GROUP}")

    # Group into "Source Files" and "Header Files"
    if ("${FILE}" MATCHES ".*\\.cpp")
       set(GROUP "Source Files${GROUP}")
    elseif("${FILE}" MATCHES ".*\\.h")
       set(GROUP "Header Files${GROUP}")
    endif()

    # Check if file exists
    if(NOT EXISTS ${FILE})
        message( FATAL_ERROR "THE FILE YOU'VE ADDED: ${FILE} DOES NOT EXIST!" )
    else()
        message ( STATUS "Group added: ${GROUP} : ${FILE}")
    endif()

    source_group("${GROUP}" FILES "${FILE}")
endforeach()

# Tell CMake to build a Farquaad executable
add_executable(Farquaad ${SOURCES})

# Include SFML
include_directories(${SFML_SOURCE_DIR}/include)
link_directories(${SFML_LIBRARY_DIR})

# Copies SFML outputs to our binary dir
set_target_properties(sfml-window PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${Farquaad_BINARY_DIR}")
set_target_properties(sfml-graphics PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${Farquaad_BINARY_DIR}")
set_target_properties(sfml-system PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${Farquaad_BINARY_DIR}")
target_link_libraries(Farquaad sfml-window sfml-graphics sfml-system)

# Include EntityX
include_directories(${EntityX_SOURCE_DIR})
link_directories(${EntityX_LIBRARY_DIR})
# Copy entityx outputs to our binary dir
set_target_properties(entityx PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${Farquaad_BINARY_DIR}")
set_target_properties(entityx_shared PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${Farquaad_BINARY_DIR}")
target_link_libraries(Farquaad entityx entityx_shared)

# Include JsonCPP
include_directories(${JsonCPP_SOURCE_DIR}/include)
link_directories(${JsonCPP_LIBRARY_DIR})
target_link_libraries(Farquaad jsoncpp_lib)
set_target_properties(jsoncpp_lib PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${Farquaad_BINARY_DIR}" FOLDER jsoncpp)
set_target_properties(jsoncpp_lib_static PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${Farquaad_BINARY_DIR}" FOLDER jsoncpp)

# Include Box2D
include_directories(${Box2D_SOURCE_DIR})
link_directories(${Box2D_LIBRARY_DIR})

# Set target direct to output
if(BOX2D_BUILD_SHARED)
    set_target_properties(Box2D_shared PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${Farquaad_BINARY_DIR}")
endif()

set_target_properties(Box2D PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${Farquaad_BINARY_DIR}")

# Statically link Box2D
target_link_libraries(Farquaad Box2D)


# Set installation directory to incl Packages
install(TARGETS Farquaad DESTINATION bin)
if(MSVC)
    if (CMAKE_BUILD_TYPE STREQUAL 'Debug')
        set(CMAKE_INSTALL_DEBUG_LIBRARIES 1)
    endif()
    include(InstallRequiredSystemLibraries)
endif()
