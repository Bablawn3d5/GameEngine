cmake_minimum_required(VERSION 3.1)

project(FarquaadExamples)

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
elseif(CMAKE_CXX_COMPILER_ID STREQUAL 'MSVC')
    # /Zi - Produces a program database (PDB) that contains type information and symbolic debugging information for use with the debugger.
    # /FS - Allows multiple cl.exe processes to write to the same .pdb file
    # /DEBUG - Enable debug during linking
    # /Od - Disables optimization
    set(CMAKE_CXX_FLAGS_DEBUG "/Zi /FS /DEBUG /Od /MDd")
    # /Ox - Full optimization
    set(CMAKE_CXX_FLAGS_RELEASE "/Ox -DNDEBUG")
    set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "/Ox /Zi /FS /DEBUG")
    # Define HAVE_ROUND for pymath.h not being defined by windows.
    add_definitions( /DHAVE_ROUND )
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
include_directories(${PYTHON_INCLUDE_DIRS})
include_directories(${Boost_INCLUDE_DIRS})
link_directories(${Boost_LIBRARY_DIR})

# Add target to copy files
file(GLOB ConfigFiles
    ${FarquaadExamples_SOURCE_DIR}/*.json
    ${FarquaadExamples_SOURCE_DIR}/Resources/*.json)

add_custom_target(CopyConfigs)
foreach(ConfigFile ${ConfigFiles})
    # Get the directory of the source file
    get_filename_component(PARENT_DIR "${ConfigFile}" DIRECTORY)
    get_filename_component(PARENT_DIR "${PARENT_DIR}" ABSOLUTE)

    # Remove common directory prefix to make the group
    string(REPLACE "${FarquaadExamples_SOURCE_DIR}" "" GROUP "${PARENT_DIR}")

    add_custom_command(TARGET CopyConfigs PRE_BUILD
                     COMMAND ${CMAKE_COMMAND} -E
                         make_directory "${CMAKE_BINARY_DIR}/${CMAKE_CFG_INTDIR}/${GROUP}/" )
    add_custom_command(TARGET CopyConfigs PRE_BUILD
                     COMMAND ${CMAKE_COMMAND} -E
                         copy "${ConfigFile}"
                              "${CMAKE_BINARY_DIR}/${CMAKE_CFG_INTDIR}/${GROUP}/"
                     DEPENDS ${ConfigFile}
                     COMMENT "Copying ${ConfigFile}" )
    install(FILES ${ConfigFile} DESTINATION bin/${GROUP})
endforeach()

add_dependencies(FarquaadExamples CopyConfigs)
target_link_libraries(FarquaadExamples Farquaad)


# Set installation directory to incl Packages
install(TARGETS FarquaadExamples DESTINATION bin)
if(MSVC)
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
