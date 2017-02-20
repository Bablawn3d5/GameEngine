cmake_minimum_required(VERSION 3.1)

project(FarquaadExamples)

# Default compiler args
if (CMAKE_CXX_COMPILER_ID MATCHES "(GNU|.*Clang)")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pedantic -Werror -Wall -Wextra -Wno-unused-parameter -Wno-error=unused-variable -Wno-error=sign-compare -std=c++14 -DBOOST_NO_AUTO_PTR -Wno-error=deprecated-declarations -Wno-unused-local-typedefs")
    if (CMAKE_CXX_COMPILER_ID MATCHES "(.*Clang)")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-missing-field-initializers -Qunused-arguments")
    endif()
    if (CMAKE_CXX_COMPILER_ID MATCHES "(GNU)")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-error=unused-local-typedefs")
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

set(SCRIPTDIR ${PROJECT_SOURCE_DIR}/scripts)

# Add target to copy files
file(GLOB ExampleResources
    ${SCRIPTDIR}/action/__init__.py
    ${SCRIPTDIR}/action/mouse.py
    ${SCRIPTDIR}/action/shooter.py
    ${SCRIPTDIR}/action/explosion.py
    ${SCRIPTDIR}/action/follower.py
    ${SCRIPTDIR}/entityx/__init__.py
    ${SCRIPTDIR}/system/__init__.py
    ${SCRIPTDIR}/system/loader.py
    ${SCRIPTDIR}/gamemath/__init__.py
    ${SCRIPTDIR}/gamemath/vector2.py
    ${SCRIPTDIR}/gamemath/util.py
    ${FarquaadExamples_SOURCE_DIR}/Config.json
    ${FarquaadExamples_SOURCE_DIR}/Resources/Loader.json
    ${FarquaadExamples_SOURCE_DIR}/Resources/Box.json
    ${FarquaadExamples_SOURCE_DIR}/Resources/Hero.json
    ${FarquaadExamples_SOURCE_DIR}/Resources/Mouse.json
    ${FarquaadExamples_SOURCE_DIR}/images/Chest_1.ase
    ${FarquaadExamples_SOURCE_DIR}/images/Bar.ase
    ${FarquaadExamples_SOURCE_DIR}/images/perfectface.png
    ${FarquaadExamples_SOURCE_DIR}/sounds/Explode.wav
    ${PYTHON27_STD_ZIP})

add_custom_target(CopyExampleResources DEPENDS ${ExampleResources})

# Set target copy and install.
foreach(ScriptFile ${ExampleResources})
    # Get the directory of the source file
    get_filename_component(PARENT_DIR "${ScriptFile}" DIRECTORY)
    get_filename_component(PARENT_DIR "${PARENT_DIR}" ABSOLUTE)

    # Remove common directory prefix to make the group
    string(REPLACE "${PROJECT_SOURCE_DIR}" "" GROUP "${PARENT_DIR}")
    string(REPLACE "${PYTHON27_STD_ZIP_DIR}" "" GROUP "${GROUP}")

    add_custom_command(TARGET CopyExampleResources PRE_BUILD
                     COMMAND ${CMAKE_COMMAND} -E
                         make_directory "${CMAKE_BINARY_DIR}/${CMAKE_CFG_INTDIR}/${GROUP}/" )
    add_custom_command(TARGET CopyExampleResources PRE_BUILD
                     COMMAND ${CMAKE_COMMAND} -E
                         copy "${ScriptFile}"
                              "${CMAKE_BINARY_DIR}/${CMAKE_CFG_INTDIR}/${GROUP}/"
                     DEPENDS ${ExampleResources}
                     COMMENT "Copying ${ScriptFile}")
    source_group("${GROUP}" FILES "${ScriptFile}")
    install(FILES ${ScriptFile} DESTINATION bin/${GROUP})
endforeach()


# Define HAVE_ROUND for pymath.h not being defined by windows.
add_definitions(/DHAVE_ROUND)
# Statically link boost
add_definitions(-DBOOST_STATIC -DBOOST_PYTHON_STATIC_LIB )

if(NOT BUILD_SHARED_LIBS)
    add_definitions(-DSFML_STATIC)
endif()
# Tell CMake to build a Farquaad executable
add_executable(FarquaadExamples main.cpp)
include_directories(${Farquaad_SOURCE_DIR}/include)
include_directories(${jsoncpp-amalgamate_SOURCE_DIR})
include_directories(${EntityX_SOURCE_DIR})
include_directories(${SFML_SOURCE_DIR}/include)
include_directories(${Box2D_SOURCE_DIR})
include_directories(${thor_SOURCE_DIR}/include)
include_directories(${thor_SOURCE_DIR}/extlibs/aurora/include/)
include_directories(${IMGUI_AMALGAMATE_INCLUDE_DIRS})
include_directories(${METASTUFF_INCLUDE_DIRS})
include_directories(${AsepriteToSFMLImage_INCLUDE_DIRS})
include_directories(${AsepriteToSFMLImage_SOURCE_DIR})
include_directories(${AsepriteToSFMLImage_SOURCE_DIR}/aseprite-rgba-converter)

# Add Python
include_directories(${PYBIND11_INCLUDE_DIR})
include_directories(${ENTITYX_PYBIND11_INCLUDE_DIR})
include_directories(${PYTHON_INCLUDE_DIRS})
target_link_libraries(FarquaadExamples ${PYTHON_LIBRARIES} ${ENTITYX_PYBIND11_LIBRARIES})

add_dependencies(FarquaadExamples CopyExampleResources)
target_link_libraries(FarquaadExamples Farquaad)


# Set installation directory to incl Packages
install(TARGETS FarquaadExamples DESTINATION bin)
if (CMAKE_BUILD_TYPE STREQUAL 'Debug')
    set(CMAKE_INSTALL_DEBUG_LIBRARIES 1)
endif()
include(InstallRequiredSystemLibraries)
