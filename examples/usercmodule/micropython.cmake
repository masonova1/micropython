# This top-level micropython.cmake is responsible for listing
# the individual modules we want to include.
# Paths are absolute, and ${CMAKE_CURRENT_LIST_DIR} can be
# used to prefix subdirectories.

include(${CMAKE_CURRENT_LIST_DIR}/engine/micropython.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/modraster/micropython.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/modbox2d/micropython.cmake)
