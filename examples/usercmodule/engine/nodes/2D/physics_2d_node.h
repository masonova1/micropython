#ifndef PHYSICS_2D_NODE_H
#define PHYSICS_2D_NODE_H

#include "py/obj.h"
#include "utility/linked_list.h"
#include "nodes/2d/physics_2d_node.h"
#include "nodes/node_base.h"

#define PHYSAC_STANDALONE
#define PHYSAC_NO_THREADS
#include "libs/physac/physac.h"

typedef struct{
    mp_obj_t position;              // Vector2: 2d xy position of this node
    mp_obj_t velocity;              // Vector2
    mp_obj_t acceleration;          // Vector2 
}engine_physics_2d_node_class_obj_t;

typedef struct{
    mp_obj_t tick_cb;
    void *box2d_shape;
    void *box2d_body;
    linked_list_node *physics_list_node;    // All physics 2d nodes get added to a list that is easy to traverse
}engine_physics_2d_node_common_data_t;

extern const mp_obj_type_t engine_physics_2d_node_class_type;

#endif  // PHYSICS_2D_NODE_H