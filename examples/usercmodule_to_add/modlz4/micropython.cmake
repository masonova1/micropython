# Create an INTERFACE library for our CPP module.
add_library(usermod_modlz4 INTERFACE)

# Add our source files to the library.
target_sources(usermod_modlz4 INTERFACE
    ${CMAKE_CURRENT_LIST_DIR}/modlz4.c
    ${CMAKE_CURRENT_LIST_DIR}/lz4.c
)

# Add the current directory as an include directory.
target_include_directories(usermod_modlz4 INTERFACE
    ${CMAKE_CURRENT_LIST_DIR}
)

# Link our INTERFACE library to the usermod target.
target_link_libraries(usermod INTERFACE usermod_modlz4)
