cmake_minimum_required(VERSION 3.1)
project(Farquaad_Tests)

set(TESTROOT ${PROJECT_SOURCE_DIR})

set(TEST_SOURCES
    ${catch-amalgamate_SOURCE_DIR}/catch.hpp
    ${TESTROOT}/main.cpp
    ${TESTROOT}/Common.h
    ${TESTROOT}/Common.cpp
    ${TESTROOT}/ComponentSeralizer_test.cpp
    ${TESTROOT}/EntitySeralizer_test.cpp
    ${TESTROOT}/JSONSerializedComponents_test.cpp
    ${TESTROOT}/JSONSerializedSystemObjects_test.cpp
)

# Add target to copy files
file(GLOB ScriptFiles
    ${TESTROOT}/scripts/entityx/*.py
    ${TESTROOT}/scripts/entityx/tests/*.py
    ${PYTHON27_STD_ZIP})

add_custom_target(CopyScriptFiles SOURCES ${ScriptFiles})
foreach(ScriptFile ${ScriptFiles})
    # Get the directory of the source file
    get_filename_component(PARENT_DIR "${ScriptFile}" DIRECTORY)
    get_filename_component(PARENT_DIR "${PARENT_DIR}" ABSOLUTE)

    # Remove common directory prefix to make the group
    string(REPLACE "${TESTROOT}" "" GROUP "${PARENT_DIR}")
    string(REPLACE "${PYTHON27_STD_ZIP_DIR}" "" GROUP "${GROUP}")

    add_custom_command(TARGET CopyScriptFiles PRE_BUILD
                     COMMAND ${CMAKE_COMMAND} -E
                         make_directory "${CMAKE_BINARY_DIR}/${CMAKE_CFG_INTDIR}/${GROUP}/" )
    add_custom_command(TARGET CopyScriptFiles PRE_BUILD
                     COMMAND ${CMAKE_COMMAND} -E
                         copy "${ScriptFile}"
                              "${CMAKE_BINARY_DIR}/${CMAKE_CFG_INTDIR}/${GROUP}/"
                     DEPENDS ${ScriptFile}
                     COMMENT "Copying ${ScriptFile}")
    source_group("${GROUP}" FILES "${ScriptFile}")
    install(FILES ${ScriptFile} DESTINATION bin/${GROUP})
endforeach()

# Include common test headers
include_directories(${TESTROOT})

# Include SFML
include_directories(${SFML_SOURCE_DIR}/include)
# Include Box2D
include_directories(${Box2D_SOURCE_DIR})
# Include thor for SFML extensions
include_directories(${thor_SOURCE_DIR}/include)
include_directories(${thor_SOURCE_DIR}/extlibs/aurora/include/)
# Include EntityX
include_directories(${EntityX_SOURCE_DIR})
# Include JsonCPP-amalgamate headers to avoid cmake issues.
include_directories(${jsoncpp-amalgamate_SOURCE_DIR})

#Add python
include_directories(${PYBIND11_INCLUDE_DIR})
include_directories(${ENTITYX_PYBIND11_INCLUDE_DIR})
include_directories(${PYTHON_INCLUDE_DIRS})

include_directories(${IMGUI_AMALGAMATE_INCLUDE_DIRS})
include_directories(${METASTUFF_INCLUDE_DIRS})
include_directories(${AsepriteToSFMLImage_SOURCE_DIR})
include_directories(${AsepriteToSFMLImage_SOURCE_DIR}/aseprite-rgba-converter)

# Build Executable
enable_testing()
# Define HAVE_ROUND for python on windows
add_definitions(/DHAVE_ROUND)
add_executable(all_tests ${TEST_SOURCES})
include_directories(${catch-amalgamate_SOURCE_DIR})
source_group("" FILES "${TEST_SOURCES}")
target_link_libraries(all_tests ${ENTITYX_LIBRARIES} ${PYTHON_LIBRARIES} ${ENTITYX_PYBIND11_LIBRARIES})
target_link_libraries(all_tests ${SFML_DEPENDENCIES} ${SFML_LIBRARIES})
target_link_libraries(all_tests thor)
target_link_libraries(
    all_tests
    Farquaad
    )
add_dependencies (all_tests Farquaad CopyScriptFiles)
set_target_properties(all_tests PROPERTIES
    DEPENDS all_tests
    FOLDER "FarquaadEngine/tests")

# Set installation directory to incl Packages
install(TARGETS all_tests DESTINATION bin)
if (CMAKE_BUILD_TYPE STREQUAL 'Debug')
    set(CMAKE_INSTALL_DEBUG_LIBRARIES 1)
endif()
include(InstallRequiredSystemLibraries)
