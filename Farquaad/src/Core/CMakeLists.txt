
set(INCROOT ${PROJECT_SOURCE_DIR}/include/Farquaad/Core)
set(SRCROOT ${PROJECT_SOURCE_DIR}/src/Core)

set(SRC
    ${SRCROOT}/EntitySerializer.cpp
    ${SRCROOT}/ComponentSerializer.cpp
)

# Include hpp
set(INC
    ${INCROOT}/MetaRegister.hpp
    ${INCROOT}/ComponentSerializer.h
    ${INCROOT}/EntitySerializer.h
    ${INCROOT}/Serializable.hpp
    ${INCROOT}/SerializableHandle.hpp
    ${INCROOT}/../Core.hpp
    ${INCROOT}/../Serialization.hpp
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
