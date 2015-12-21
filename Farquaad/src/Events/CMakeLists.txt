
set(INCROOT ${PROJECT_SOURCE_DIR}/include/Farquaad/Events)
set(SRCROOT ${PROJECT_SOURCE_DIR}/src/Events)

set(SRC
    ${INCROOT}/CollisionEvent.hpp
)

# Set include == src for Events
set(INC
    ${SRC}
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
