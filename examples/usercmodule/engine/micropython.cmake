# Create an INTERFACE library for our CPP module..
add_library(usermod_engine INTERFACE)

# Add our source files to the lib
target_sources(usermod_engine INTERFACE
    ${CMAKE_CURRENT_LIST_DIR}/engine.c
    ${CMAKE_CURRENT_LIST_DIR}/nodes/empty_node.c
    ${CMAKE_CURRENT_LIST_DIR}/utility/linked_list.c
    ${CMAKE_CURRENT_LIST_DIR}/engine_object_layers.c
)

# Add the current directory as an include directory.
target_include_directories(usermod_engine INTERFACE
    ${CMAKE_CURRENT_LIST_DIR}
)

# Link our INTERFACE library to the usermod target.
target_link_libraries(usermod INTERFACE usermod_engine)