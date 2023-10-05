#ifndef NODE_H
#define NODE_H

// MicroPython header files: https://micropython-usermod.readthedocs.io/en/latest/usermods_05.html#header-files
#include "py/obj.h"
#include "py/runtime.h"
#include "py/builtin.h"

#include <cstdint>
#include <vector>
#include <stdexcept>

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

        // The parent node of this node
        mp_obj_t* parent;

        // List of child nodes under this node
        std::vector<mp_obj_t*> children;

        // Callback functions that are invoked by the game engine
        // upon calling Engine::start():
        //  * node.tick(): called
        void (*tick)();
        void (*physics_tick)();
    private:
        // The index of this node in its parent's 'children'
        // Makes it easy for this node to delete itself
        // from parent's child list without looping
        uint16_t index_in_parent_child_list = 0;
};


#endif  // NODE_H