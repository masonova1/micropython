CALLBACK_TEST_MOD_DIR := $(USERMOD_DIR)

# Add our source files to the respective variables.
SRC_USERMOD += $(CALLBACK_TEST_MOD_DIR)/engine_module.c
SRC_USERMOD_CXX += $(CALLBACK_TEST_MOD_DIR)/engine.cpp
SRC_USERMOD_CXX += $(CALLBACK_TEST_MOD_DIR)/node_2d_physics.cpp
SRC_USERMOD_CXX += $(CALLBACK_TEST_MOD_DIR)/node_2d_bitmap_sprite.cpp
SRC_USERMOD_CXX += $(CALLBACK_TEST_MOD_DIR)/node.cpp

# We can add our module folder to include paths if needed
# This is not actually needed in this example.
CFLAGS_USERMOD += -I$(CALLBACK_TEST_MOD_DIR)
CXXFLAGS_USERMOD += -I$(CALLBACK_TEST_MOD_DIR) -std=c++11

# We use C++ features so have to link against the standard library.
LDFLAGS_USERMOD += -lstdc++

