#include "rectangle.h"
#include "utility/debug_print.h"

// Class required functions
STATIC void rectangle_class_print(const mp_print_t *print, mp_obj_t self_in, mp_print_kind_t kind){
    rectangle_class_obj_t *self = self_in;
    ENGINE_INFO_PRINTF("print(): rectangle [x: %0.3f, y: %0.3f, width: %0.3f, height: %0.3f]", (double)self->pos.x, (double)self->pos.y, (double)self->size.x, (double)self->size.y);
}

mp_obj_t rectangle_class_new(const mp_obj_type_t *type, size_t n_args, size_t n_kw, const mp_obj_t *args){
    ENGINE_INFO_PRINTF("New rectangle");

    rectangle_class_obj_t *self = m_new_obj(rectangle_class_obj_t);
    self->base.type = &rectangle_class_type;

    if(n_args == 0) {
      self->base.type = &rectangle_class_type;
      self->pos.x = 0.f;
      self->pos.y = 0.f;
      self->size.x = 0.f;
      self->size.y = 0.f;
  } else if(n_args == 2) {
      if(!mp_obj_is_type(args[0], &vector2_class_type) || !mp_obj_is_type(args[1], &vector2_class_type)){
          mp_raise_TypeError("expected vector arguments");
      }
      self->base.type = &rectangle_class_type;
      self->pos = *((vector2_class_obj_t*)MP_OBJ_TO_PTR(args[0]));
      self->size = *((vector2_class_obj_t*)MP_OBJ_TO_PTR(args[1]));
    } else {
      mp_raise_TypeError("function takes 0 or 2 arguments");
    }

    return MP_OBJ_FROM_PTR(self);
}


// Class methods
STATIC mp_obj_t rectangle_class_area(mp_obj_t self_in){
    const rectangle_class_obj_t* self = MP_OBJ_TO_PTR(self_in);
    return mp_obj_new_float(self->size.x * self->size.y);
}
MP_DEFINE_CONST_FUN_OBJ_1(rectangle_class_area_obj, rectangle_class_area);

// Class methods
STATIC mp_obj_t rectangle_class_overlap_test(mp_obj_t self_in, mp_obj_t b_in){
    const rectangle_class_obj_t* self = MP_OBJ_TO_PTR(self_in);
    const rectangle_class_obj_t* b = MP_OBJ_TO_PTR(b_in);
    if(b->pos.x + b->size.x < self->pos.x) return mp_const_false;
    else if(self->pos.x + self->size.x < b->pos.x) return mp_const_false;
    else if(b->pos.y + b->size.y < self->pos.y) return mp_const_false;
    else if(self->pos.y + self->size.y < b->pos.y) return mp_const_false;
    else return mp_const_true;
}
MP_DEFINE_CONST_FUN_OBJ_2(rectangle_class_overlap_test_obj, rectangle_class_overlap_test);


// Function called when accessing like print(my_node.position.x) (load 'x')
// my_node.position.x = 0 (store 'x').
// See https://micropython-usermod.readthedocs.io/en/latest/usermods_09.html#properties
// See https://github.com/micropython/micropython/blob/91a3f183916e1514fbb8dc58ca5b77acc59d4346/extmod/modasyncio.c#L227
STATIC void rectangle_class_attr(mp_obj_t self_in, qstr attribute, mp_obj_t *destination){
    rectangle_class_obj_t *self = MP_OBJ_TO_PTR(self_in);

    if(destination[0] == MP_OBJ_NULL){          // Load
        switch(attribute) {
            case MP_QSTR_pos:
                destination[0] = m_new_obj(vector2_class_obj_t);
                ((vector2_class_obj_t*)MP_OBJ_TO_PTR(destination[0]))->base.type = &vector2_class_type;
                ((vector2_class_obj_t*)MP_OBJ_TO_PTR(destination[0]))->x = self->pos.x;
                ((vector2_class_obj_t*)MP_OBJ_TO_PTR(destination[0]))->y = self->pos.y;
                break;
            case MP_QSTR_size:
                destination[0] = m_new_obj(vector2_class_obj_t);
                ((vector2_class_obj_t*)MP_OBJ_TO_PTR(destination[0]))->base.type = &vector2_class_type;
                ((vector2_class_obj_t*)MP_OBJ_TO_PTR(destination[0]))->x = self->size.x;
                ((vector2_class_obj_t*)MP_OBJ_TO_PTR(destination[0]))->y = self->size.y;
                break;
            case MP_QSTR_area: destination[0] = MP_OBJ_FROM_PTR(&rectangle_class_area_obj); destination[1] = self_in; break;
            default: break;
        }
    }else if(destination[1] != MP_OBJ_NULL){    // Store
        switch(attribute) {
            case MP_QSTR_pos: self->pos = *((vector2_class_obj_t*)MP_OBJ_TO_PTR(destination[1])); break;
            case MP_QSTR_size: self->size = *((vector2_class_obj_t*)MP_OBJ_TO_PTR(destination[1])); break;
            default: return;
        }

        // Success
        destination[0] = MP_OBJ_NULL;
    }
}


// Class attributes
STATIC const mp_rom_map_elem_t rectangle_class_locals_dict_table[] = {
    { MP_ROM_QSTR(MP_QSTR_area), MP_ROM_PTR(&rectangle_class_area_obj) },
    { MP_ROM_QSTR(MP_QSTR_overlap_test), MP_ROM_PTR(&rectangle_class_overlap_test_obj) },
};


// Class init
STATIC MP_DEFINE_CONST_DICT(rectangle_class_locals_dict, rectangle_class_locals_dict_table);

const mp_obj_type_t rectangle_class_type = {
    { &mp_type_type },
    .name = MP_QSTR_Rectangle,
    .print = rectangle_class_print,
    .make_new = rectangle_class_new,
    .call = NULL,
    .unary_op = NULL,
    .binary_op = NULL,
    .attr = rectangle_class_attr,
    .subscr = NULL,
    .getiter = NULL,
    .iternext = NULL,
    .buffer_p = {NULL},
    .locals_dict = (mp_obj_dict_t*)&rectangle_class_locals_dict,
};
