
set(INCROOT ${PROJECT_SOURCE_DIR}/include/Farquaad/Systems)
set(SRCROOT ${PROJECT_SOURCE_DIR}/src/Systems)

set(SRC
    ${SRCROOT}/InputSystem.cpp
)

set(INC
    ${INCROOT}/InputSystem.h
    ${INCROOT}/../Systems.hpp
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
