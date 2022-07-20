# Create an INTERFACE library for our C module.
add_library(usermod_emulator INTERFACE)

# Add our source files to the lib
target_sources(usermod_emulator INTERFACE
    ${CMAKE_CURRENT_LIST_DIR}/emulatormodule.c
)

# Add the current directory as an include directory.
target_include_directories(usermod_emulator INTERFACE
    ${CMAKE_CURRENT_LIST_DIR}
)

# Link our INTERFACE library to the usermod target.
target_link_libraries(usermod INTERFACE usermod_emulator)
