#pragma once
// I refuse to name this file "wrapped_joints.h".

typedef struct box2d_b2Joint_obj_t
{
  mp_obj_base_t base;
  b2JointId joint;
} box2d_b2Joint_obj_t;

mp_obj_t box2d_b2Joint_make_new(const mp_obj_type_t *type, size_t n_args, size_t n_kw, const mp_obj_t *args);

STATIC void box2d_b2Joint_print(const mp_print_t *print, mp_obj_t self_in, mp_print_kind_t kind) {
    (void)kind;
    box2d_b2Joint_obj_t *self = MP_OBJ_TO_PTR(self_in);
    mp_printf(print, "<b2Joint index=%u>", (int)self->joint.index);
}

STATIC const mp_rom_map_elem_t box2d_b2Joint_locals_dict_table[] = {
  {MP_ROM_QSTR(MP_QSTR_write), MP_ROM_PTR(NULL)},
};
STATIC MP_DEFINE_CONST_DICT(box2d_b2Joint_locals_dict, box2d_b2Joint_locals_dict_table);

const mp_obj_type_t box2d_b2Joint_type = {
    {&mp_type_type},
    .name = MP_QSTR_b2Joint,
    .print = box2d_b2Joint_print,
    .make_new = box2d_b2Joint_make_new,
    .locals_dict = (mp_obj_dict_t *)&box2d_b2Joint_locals_dict,
};

mp_obj_t box2d_b2Joint_make_new(const mp_obj_type_t *type,
    size_t n_args,
    size_t n_kw,
    const mp_obj_t *all_args) {

    enum {
        ARG_world,
        ARG_def,
    };
    static const mp_arg_t allowed_args[] = {
        // {MP_QSTR_world, MP_ARG_OBJ | MP_ARG_REQUIRED, {.u_obj = MP_OBJ_NULL}},
        // {MP_QSTR_def, MP_ARG_OBJ | MP_ARG_REQUIRED, {.u_obj = MP_OBJ_NULL}},
    };
    mp_arg_val_t args[MP_ARRAY_SIZE(allowed_args)];
    mp_arg_parse_all_kw_array(n_args, n_kw, all_args, MP_ARRAY_SIZE(allowed_args), allowed_args, args);

    box2d_b2Joint_obj_t *self = m_new_obj(box2d_b2Joint_obj_t);
    // box2d_b2World_obj_t* world_obj = (box2d_b2World_obj_t *)MP_OBJ_TO_PTR(args[ARG_world].u_obj);
    // box2d_b2BodyDef_obj_t* def_obj = (box2d_b2BodyDef_obj_t *)MP_OBJ_TO_PTR(args[ARG_def].u_obj);
    self->base.type = &box2d_b2Joint_type;
    //self->body = b2World_CreateBody(world_obj->world, &def_obj->def);
    return MP_OBJ_FROM_PTR(self);
}
