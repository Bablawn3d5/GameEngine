
set(INCROOT ${PROJECT_SOURCE_DIR}/include/Farquaad/Thor)
set(SRCROOT ${PROJECT_SOURCE_DIR}/src/Thor)

set(SRC
	${INCROOT}/ResourceLoader.hpp
)

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
