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
    ${TESTROOT}/PythonSeralizer_test.cpp
)

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
# Add Boost Python
include_directories(${PYTHON_INCLUDE_DIRS})
include_directories(${Boost_INCLUDE_DIRS})
link_directories(${Boost_LIBRARY_DIR})

# Build Executable
# Define HAVE_ROUND for python on windows
enable_testing()
add_definitions(/DHAVE_ROUND)
add_executable(all_tests ${TEST_SOURCES})
include_directories(${catch-amalgamate_SOURCE_DIR})
source_group("" FILES "${TEST_SOURCES}")
target_link_libraries(
    all_tests
    Farquaad
    )
add_dependencies (all_tests Farquaad)
set_target_properties(all_tests PROPERTIES
    DEPENDS all_tests
    FOLDER "FarquaadEngine/tests")

# Set installation directory to incl Packages
install(TARGETS all_tests DESTINATION bin/tests/)
if(MSVC)
    # Copy Boost DLLs
    get_filename_component(UTF_BASE_NAME
          ${Boost_PYTHON_LIBRARY_RELEASE} NAME_WE)
    get_filename_component(UTF_PATH
          ${Boost_PYTHON_LIBRARY_RELEASE} PATH)
        install(FILES ${UTF_PATH}/${UTF_BASE_NAME}.dll
          DESTINATION bin/tests/
          CONFIGURATIONS Release RelWithDebInfo
        )

    get_filename_component(UTF_BASE_NAME_DEBUG
          ${Boost_PYTHON_LIBRARY_DEBUG} NAME_WE)
        install(FILES ${UTF_PATH}/${UTF_BASE_NAME_DEBUG}.dll
          DESTINATION bin/tests/
          CONFIGURATIONS Debug
        )

    if (CMAKE_BUILD_TYPE STREQUAL 'Debug')
        set(CMAKE_INSTALL_DEBUG_LIBRARIES 1)
    endif()
    include(InstallRequiredSystemLibraries)
endif()
