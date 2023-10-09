#ifndef NODE_HPP
#define NODE_HPP

// MicroPython header files: https://micropython-usermod.readthedocs.io/en/latest/usermods_05.html#header-files
// Need to extern C so that function names can still be found
extern "C"{
    #include "py/obj.h"
    #include "py/runtime.h"
    #include "py/builtin.h"
}

#include <cstdint>
#include <vector>
#include <stdexcept>
#include <engine.hpp>
#include <node_types.h>

// Base node that all other nodes inherit.
// All nodes will have these attributes
class Node{
    public:
        Node();
        ~Node();

        // Position of node. Only some types of nodes will use
        // the Z dimension. When the node is a child of another
        // node the postion is relative to the parent node's
        // position. If the node is not a child, the postion
        // is in absolute world/scene space
        mp_int_t x;
        mp_int_t y;
        mp_int_t z;

        void set_tick_callback(mp_obj_t* new_tick_callback);
        void set_physics_tick_callback(mp_obj_t* new_physics_tick_callback);

        void set_parent(mp_obj_t* new_parent);
        void unparent();
        mp_obj_t* get_parent();

        void add_child(mp_obj_t* new_child);
        void remove_child(uint8_t child_index);
        void remove_all_children();

        uint8_t get_node_type();
    private:
        // Callback functions that are invoked by the game engine
        // upon calling Engine::start():
        //  * node.tick(): called
        void (*tick)();
        void (*physics_tick)();

        // The parent node of this node
        mp_obj_t* parent;

        // List of child nodes under this node
        std::vector<mp_obj_t*> children;

        // The index of this node in its parent's 'children'
        // Makes it easy for this node to delete itself
        // from parent's child list without finding itself
        uint16_t index_in_parent_child_list = 0;

        // The type 
        // uint8_t node_type = NODE;
};


#endif  // NODE_HPP