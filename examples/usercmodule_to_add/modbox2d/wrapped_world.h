#pragma once
#include "include/box2d/box2d.h"

typedef struct box2d_b2WorldDef_obj_t
{
  mp_obj_base_t base;
  b2WorldDef def;
} box2d_b2WorldDef_obj_t;

mp_obj_t box2d_b2WorldDef_make_new(const mp_obj_type_t *type, size_t n_args, size_t n_kw, const mp_obj_t *args);

STATIC void box2d_b2WorldDef_print(const mp_print_t *print, mp_obj_t self_in, mp_print_kind_t kind) {
    (void)kind;
    box2d_b2WorldDef_obj_t *self = MP_OBJ_TO_PTR(self_in);
    mp_printf(print, "<b2WorldDef capacity=%u>", self->def.shapeCapacity);
}

STATIC const mp_rom_map_elem_t box2d_b2WorldDef_locals_dict_table[] = {
  {MP_ROM_QSTR(MP_QSTR_write), MP_ROM_PTR(NULL)},
};
STATIC MP_DEFINE_CONST_DICT(box2d_b2WorldDef_locals_dict, box2d_b2WorldDef_locals_dict_table);

const mp_obj_type_t box2d_b2WorldDef_type = {
    {&mp_type_type},
    .name = MP_QSTR_b2WorldDef,
    .print = box2d_b2WorldDef_print,
    .make_new = box2d_b2WorldDef_make_new,
    .locals_dict = (mp_obj_dict_t *)&box2d_b2WorldDef_locals_dict,
};

mp_obj_t box2d_b2WorldDef_make_new(const mp_obj_type_t *type,
    size_t n_args,
    size_t n_kw,
    const mp_obj_t *all_args) {

    enum {
        ARG_gravity,
        ARG_enableSleep,
        ARG_shapeCapacity,
        ARG_contactCapacity,
        ARG_jointCapacity,
        ARG_stackAllocatorCapacity,
    };
    static const mp_arg_t allowed_args[] = {
        {MP_QSTR_gravity, MP_ARG_OBJ | MP_ARG_KW_ONLY, {.u_obj = NULL}},
        {MP_QSTR_enableSleep, MP_ARG_INT | MP_ARG_KW_ONLY, {.u_int = 1}},
        {MP_QSTR_shapeCapacity, MP_ARG_INT | MP_ARG_KW_ONLY, {.u_int = 8}},
        {MP_QSTR_contactCapacity, MP_ARG_INT | MP_ARG_KW_ONLY, {.u_int = 8}},
        {MP_QSTR_jointCapacity, MP_ARG_INT | MP_ARG_KW_ONLY, {.u_int = 8}},
        {MP_QSTR_stackAllocatorCapacity, MP_ARG_INT | MP_ARG_KW_ONLY, {.u_int = 24*1024}},
    };
    mp_arg_val_t args[MP_ARRAY_SIZE(allowed_args)];
    mp_arg_parse_all_kw_array(n_args, n_kw, all_args, MP_ARRAY_SIZE(allowed_args), allowed_args, args);

    box2d_b2WorldDef_obj_t *self = m_new_obj(box2d_b2WorldDef_obj_t);
    self->base.type = &box2d_b2WorldDef_type;
    box2d_b2Vec2_obj_t* gravity_obj = (box2d_b2Vec2_obj_t *)MP_OBJ_TO_PTR(args[ARG_gravity].u_obj);
    self->def = b2DefaultWorldDef();
    if(gravity_obj != NULL) self->def.gravity = gravity_obj->vec;
    self->def.enableSleep = args[ARG_enableSleep].u_int;
    self->def.shapeCapacity = args[ARG_shapeCapacity].u_int;
    self->def.contactCapacity = args[ARG_contactCapacity].u_int;
    self->def.jointCapacity = args[ARG_jointCapacity].u_int;
    self->def.stackAllocatorCapacity = args[ARG_stackAllocatorCapacity].u_int;
    return MP_OBJ_FROM_PTR(self);
}

typedef struct box2d_b2World_obj_t
{
  mp_obj_base_t base;
  b2WorldId world;
} box2d_b2World_obj_t;

mp_obj_t box2d_b2World_make_new(const mp_obj_type_t *type, size_t n_args, size_t n_kw, const mp_obj_t *args);

STATIC void box2d_b2World_print(const mp_print_t *print, mp_obj_t self_in, mp_print_kind_t kind) {
    (void)kind;
    box2d_b2World_obj_t *self = MP_OBJ_TO_PTR(self_in);
    mp_printf(print, "<b2World index=%u>", self->world.index);
}

STATIC mp_obj_t box2d_b2World_Step(size_t n_args, const mp_obj_t *args) {
    box2d_b2World_obj_t *self = MP_OBJ_TO_PTR(args[0]);
    const float dt = mp_obj_get_float(args[1]);
    const int vel_iters = mp_obj_get_int(args[2]);
    const int pos_iters = mp_obj_get_int(args[3]);
    b2World_Step(self->world, dt, vel_iters, pos_iters);
    return mp_const_none;
}
STATIC MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(box2d_b2World_Step_obj, 4, 4, box2d_b2World_Step);

STATIC mp_obj_t box2d_b2World_EnableSleeping(size_t n_args, const mp_obj_t *args) {
    box2d_b2World_obj_t *self = MP_OBJ_TO_PTR(args[0]);
    const int enableSleep = mp_obj_get_int(args[1]);
    b2World_EnableSleeping(self->world, enableSleep);
    return mp_const_none;
}
STATIC MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(box2d_b2World_EnableSleeping_obj, 2, 2, box2d_b2World_EnableSleeping);

// STATIC mp_obj_t box2d_b2World_EnableContinuous(size_t n_args, const mp_obj_t *args) {
//     box2d_b2World_obj_t *self = MP_OBJ_TO_PTR(args[0]);
//     const int enableContinuous = mp_obj_get_int(args[1]);
//     b2World_EnableContinuous(self->world, enableContinuous);
//     return mp_const_none;
// }
// STATIC MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(box2d_b2World_EnableContinuous_obj, 2, 2, box2d_b2World_EnableContinuous);

STATIC const mp_rom_map_elem_t box2d_b2World_locals_dict_table[] = {
  {MP_ROM_QSTR(MP_QSTR_Step), MP_ROM_PTR(&box2d_b2World_Step_obj)},
  {MP_ROM_QSTR(MP_QSTR_EnableSleep), MP_ROM_PTR(&box2d_b2World_EnableSleeping_obj)},
  //{MP_ROM_QSTR(MP_QSTR_enablecontinuous), MP_ROM_PTR(&box2d_b2World_EnableContinuous_obj)},
};
STATIC MP_DEFINE_CONST_DICT(box2d_b2World_locals_dict, box2d_b2World_locals_dict_table);

const mp_obj_type_t box2d_b2World_type = {
    {&mp_type_type},
    .name = MP_QSTR_b2World,
    .print = box2d_b2World_print,
    .make_new = box2d_b2World_make_new,
    .locals_dict = (mp_obj_dict_t *)&box2d_b2World_locals_dict,
};

mp_obj_t box2d_b2World_make_new(const mp_obj_type_t *type,
    size_t n_args,
    size_t n_kw,
    const mp_obj_t *all_args) {

    enum {
        ARG_def,
    };
    mp_printf(&mp_plat_print, "Creating world");
    static const mp_arg_t allowed_args[] = {
        {MP_QSTR_def, MP_ARG_OBJ | MP_ARG_REQUIRED, {.u_obj = MP_OBJ_NULL}},
    };
    mp_arg_val_t args[MP_ARRAY_SIZE(allowed_args)];
    mp_arg_parse_all_kw_array(n_args, n_kw, all_args, MP_ARRAY_SIZE(allowed_args), allowed_args, args);

    box2d_b2World_obj_t *self = m_new_obj(box2d_b2World_obj_t);
    box2d_b2WorldDef_obj_t* def_obj = (box2d_b2WorldDef_obj_t *)MP_OBJ_TO_PTR(args[ARG_def].u_obj);
    self->base.type = &box2d_b2World_type;
    self->world = b2CreateWorld(&(def_obj->def));
    return MP_OBJ_FROM_PTR(self);
}
