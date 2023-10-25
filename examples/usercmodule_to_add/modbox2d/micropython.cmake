# Create an INTERFACE library for our CPP module.
add_library(usermod_modbox2d INTERFACE)

# Add our source files to the library.
target_sources(usermod_modbox2d INTERFACE
    ${CMAKE_CURRENT_LIST_DIR}/modbox2d.c
    ${CMAKE_CURRENT_LIST_DIR}/src/allocate.c
    ${CMAKE_CURRENT_LIST_DIR}/src/array.c
    ${CMAKE_CURRENT_LIST_DIR}/src/bitset.c
    ${CMAKE_CURRENT_LIST_DIR}/src/block_allocator.c
    ${CMAKE_CURRENT_LIST_DIR}/src/body.c
    ${CMAKE_CURRENT_LIST_DIR}/src/broad_phase.c
    ${CMAKE_CURRENT_LIST_DIR}/src/contact.c
    ${CMAKE_CURRENT_LIST_DIR}/src/contact_solver.c
    ${CMAKE_CURRENT_LIST_DIR}/src/core.c
    ${CMAKE_CURRENT_LIST_DIR}/src/distance.c
    ${CMAKE_CURRENT_LIST_DIR}/src/dynamic_tree.c
    ${CMAKE_CURRENT_LIST_DIR}/src/geometry.c
    ${CMAKE_CURRENT_LIST_DIR}/src/hull.c
    ${CMAKE_CURRENT_LIST_DIR}/src/island.c
    ${CMAKE_CURRENT_LIST_DIR}/src/joint.c
    ${CMAKE_CURRENT_LIST_DIR}/src/manifold.c
    ${CMAKE_CURRENT_LIST_DIR}/src/math.c
    ${CMAKE_CURRENT_LIST_DIR}/src/mouse_joint.c
    ${CMAKE_CURRENT_LIST_DIR}/src/pool.c
    ${CMAKE_CURRENT_LIST_DIR}/src/revolute_joint.c
    ${CMAKE_CURRENT_LIST_DIR}/src/shape.c
    ${CMAKE_CURRENT_LIST_DIR}/src/stack_allocator.c
    ${CMAKE_CURRENT_LIST_DIR}/src/table.c
    ${CMAKE_CURRENT_LIST_DIR}/src/timer.c
    ${CMAKE_CURRENT_LIST_DIR}/src/types.c
    ${CMAKE_CURRENT_LIST_DIR}/src/world.c
)

# Add the current directory as an include directory.
target_include_directories(usermod_modbox2d INTERFACE
    ${CMAKE_CURRENT_LIST_DIR}
    ${CMAKE_CURRENT_LIST_DIR}/include/
)

# Link our INTERFACE library to the usermod target.
target_link_libraries(usermod INTERFACE usermod_modbox2d)
