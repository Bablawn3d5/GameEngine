
# Process all directories
add_subdirectory(${SRCROOT}/Components)
add_subdirectory(${SRCROOT}/Systems)
add_subdirectory(${SRCROOT}/Events)
add_subdirectory(${SRCROOT}/Core)
add_subdirectory(${SRCROOT}/Thor)
add_subdirectory(${SRCROOT}/Box2D)

# Pass sources and includes to parent
set(SOURCES
    ${SOURCES}
    PARENT_SCOPE
)

set(INCLUDES
    ${INCLUDES}
    PARENT_SCOPE
)
