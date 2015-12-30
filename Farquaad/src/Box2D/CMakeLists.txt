
set(INCROOT ${PROJECT_SOURCE_DIR}/include/Farquaad/Box2D)
set(SRCROOT ${PROJECT_SOURCE_DIR}/src/Box2D)

set(SRC
    ${SRCROOT}/SFMLB2DDebug.cpp
)

set(INC
    ${INCROOT}/SFMLB2DDebug.h
)

# Set all source files
set(SOURCES
    ${SOURCES}
    ${SRC}
    PARENT_SCOPE
)

# Set all include files
set(INCLUDES
    ${INCLUDES}
    ${INC}
    PARENT_SCOPE
)
