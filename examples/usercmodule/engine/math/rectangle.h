#ifndef RECTANGLE_H
#define RECTANGLE_H

#include "py/obj.h"

#include "vector2.h"

typedef struct{
    mp_obj_base_t base;
    mp_obj_t pos; // Position (x, y)
    mp_obj_t size; // Size (width, height)
}rectangle_class_obj_t;

extern const mp_obj_type_t rectangle_class_type;

mp_obj_t rectangle_class_new(const mp_obj_type_t *type, size_t n_args, size_t n_kw, const mp_obj_t *args);

#endif  // RECTANGLE_H
