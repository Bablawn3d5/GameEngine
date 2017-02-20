
set(INCROOT ${PROJECT_SOURCE_DIR}/include/Farquaad/Components)
set(SRCROOT ${PROJECT_SOURCE_DIR}/src/Components)

set(SRC
    ${INCROOT}/Body.hpp
    ${INCROOT}/Destroyed.hpp
    ${INCROOT}/Physics.hpp
    ${INCROOT}/InputResponder.hpp
    ${INCROOT}/Stats.hpp
    ${INCROOT}/Sound.hpp
    ${INCROOT}/Renderable.hpp
)

set(INC
    ${INCROOT}/../Components.hpp
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
