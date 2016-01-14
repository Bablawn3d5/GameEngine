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
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pedantic -Werror -Wall -Wextra -Wno-unused-parameter -Wno-error=unused-variable -Wno-error=sign-compare -std=c++11 -DBOOST_NO_AUTO_PTR -Wno-error=deprecated-declarations -Wno-unused-local-typedefs")
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
elseif(MAKE_CXX_COMPILER_ID STREQUAL 'MSVC')
    # Define HAVE_ROUND for pymath.h
    # FIXME(SMA) : This doesn't correctly include the macro definition for
    # some reason???
    add_definitions(-DHAVE_ROUND)
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
set(TESTROOT ${PROJECT_SOURCE_DIR}/tests)

set(
    SOURCES
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
    elseif("${FILE}" MATCHES ".*\\.h(pp)?")
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

# Tell CMake to build a Farquaad library
# HACK(SMA) : We're mis-linking dynamic and static libs somehwere.
# We should fix this eventually.
add_library(Farquaad STATIC ${SOURCES} ${INCLUDES})

#Add tests
enable_testing()

macro(create_test TARGET_NAME SOURCE)
    set(TEST_SOURCES
        ${SOURCE}
        ${catch-amalgamate_SOURCE_DIR}/catch.hpp
        ${TESTROOT}/main.cpp)
    add_executable(${TARGET_NAME} ${TEST_SOURCES})
    include_directories(${catch-amalgamate_SOURCE_DIR})
    source_group("" FILES "${TEST_SOURCES}")
    target_link_libraries(
        ${TARGET_NAME}
        Farquaad
        ${ARGN}
        )
    add_dependencies (${TARGET_NAME} Farquaad catchmain)
    add_test(NAME ${TARGET_NAME}
             WORKING_DIRECTORY ${CMAKE_PROJECT_BINARY_DIR}
             COMMAND ${TARGET_NAME})
    set_target_properties(${TARGET_NAME} PROPERTIES DEPENDS ${TARGET_NAME} FOLDER "FarquaadEngine/tests")
endmacro()

add_executable(catchmain ${TESTROOT}/main.cpp)
set_target_properties(catchmain PROPERTIES DEPENDS catchmain FOLDER "FarquaadEngine/tests")

create_test(ComponentSeralizer_test ${TESTROOT}/ComponentSeralizer_test.cpp)
create_test(EntitySeralizer_test ${TESTROOT}/EntitySeralizer_test.cpp)
create_test(JSONSerializedComponents_test ${TESTROOT}/JSONSerializedComponents_test.cpp)
create_test(JSONSerializedSystemObjects_test ${TESTROOT}/JSONSerializedSystemObjects_test.cpp)
create_test(PythonSeralizer_test ${TESTROOT}/PythonSeralizer_test.cpp)

# Include SFML
include_directories(${SFML_SOURCE_DIR}/include)
target_link_libraries(Farquaad sfml-window sfml-graphics sfml-system)

# Include thor for SFML extensions
include_directories(${thor_SOURCE_DIR}/include)
include_directories(${thor_SOURCE_DIR}/extlibs/aurora/include/)
target_link_libraries(Farquaad thor)

# Include EntityX
include_directories(${EntityX_SOURCE_DIR})
target_link_libraries(Farquaad entityx)

# Include JsonCPP-amalgamate headers to avoid cmake issues.
include_directories(${jsoncpp-amalgamate_SOURCE_DIR})
target_link_libraries(Farquaad jsoncpp-amalgamate)
set_target_properties(jsoncpp-amalgamate PROPERTIES FOLDER "JsonCPP")

# Include Box2D
include_directories(${Box2D_SOURCE_DIR})
target_link_libraries(Farquaad Box2D)

# Add Boost Python
include_directories(${PYTHON_INCLUDE_DIRS})
include_directories(${Boost_INCLUDE_DIRS})
link_directories(${Boost_LIBRARY_DIR})
target_link_libraries(Farquaad ${Boost_LIBRARIES} ${PYTHON_LIBRARIES})

# Set installation directory to incl Packages
install(TARGETS Farquaad DESTINATION lib)
if(MSVC)
    # Copy Boost DLLs
    get_filename_component(UTF_BASE_NAME
          ${Boost_PYTHON_LIBRARY_RELEASE} NAME_WE)
    get_filename_component(UTF_PATH
          ${Boost_PYTHON_LIBRARY_RELEASE} PATH)
        install(FILES ${UTF_PATH}/${UTF_BASE_NAME}.dll
          DESTINATION bin
          CONFIGURATIONS Release RelWithDebInfo
        )

    get_filename_component(UTF_BASE_NAME_DEBUG
          ${Boost_PYTHON_LIBRARY_DEBUG} NAME_WE)
        install(FILES ${UTF_PATH}/${UTF_BASE_NAME_DEBUG}.dll
          DESTINATION bin
          CONFIGURATIONS Debug
        )

    if (CMAKE_BUILD_TYPE STREQUAL 'Debug')
        set(CMAKE_INSTALL_DEBUG_LIBRARIES 1)
    endif()
    include(InstallRequiredSystemLibraries)
endif()
