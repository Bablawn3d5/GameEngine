
set(INCROOT ${PROJECT_SOURCE_DIR}/include/Farquaad/Core)
set(SRCROOT ${PROJECT_SOURCE_DIR}/src/Core)

set(SRC
    ${SRCROOT}/ComponentSeralizer.cpp
)

# Include hpp
set(INC
    ${INCROOT}/Seralizeable.hpp
    ${INCROOT}/ComponentSeralizer.h
    ${INCROOT}/../Core.hpp
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
