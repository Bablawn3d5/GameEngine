
set(INCROOT ${PROJECT_SOURCE_DIR}/include/Farquaad/Core)
set(SRCROOT ${PROJECT_SOURCE_DIR}/src/Core)

set(SRC
    ${SRCROOT}/ComponentSerializer.cpp
    ${SRCROOT}/SeralizeableComponentMap.cpp
)

# Include hpp
set(INC
    ${INCROOT}/ComponentSerializer.h
    ${INCROOT}/Serializable.hpp
    ${INCROOT}/SeralizeableComponentMap.h
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
