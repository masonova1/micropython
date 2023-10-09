#include "node.hpp"


// uint8_t Node::get_node_type(){
//     return 
// }


extern "C" {
    #include <node_module.h>

    mp_obj_t node_new(){
        return mp_const_none;
    }
}