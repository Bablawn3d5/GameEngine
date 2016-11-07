
set(INCROOT ${PROJECT_SOURCE_DIR}/include/Farquaad/Thor)
set(SRCROOT ${PROJECT_SOURCE_DIR}/src/Thor)

set(SRC
	${SRCROOT}/ResourceLoader.cpp
)

set(INC
	${INCROOT}/ResourceLoader.h
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
