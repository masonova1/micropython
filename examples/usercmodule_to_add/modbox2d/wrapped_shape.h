#pragma once

mp_obj_t box2d_b2ShapeDef_make_new(const mp_obj_type_t *type, size_t n_args, size_t n_kw, const mp_obj_t *args);

STATIC void box2d_b2ShapeDef_print(const mp_print_t *print, mp_obj_t self_in, mp_print_kind_t kind) {
    (void)kind;
    box2d_b2ShapeDef_obj_t *self = MP_OBJ_TO_PTR(self_in);
    mp_printf(print, "<b2ShapeDef friction=%f, restitution=%f, density=%f>", self->def.friction, self->def.restitution, self->def.density);
}

STATIC const mp_rom_map_elem_t box2d_b2ShapeDef_locals_dict_table[] = {
  {MP_ROM_QSTR(MP_QSTR_write), MP_ROM_PTR(NULL)},
};
STATIC MP_DEFINE_CONST_DICT(box2d_b2ShapeDef_locals_dict, box2d_b2ShapeDef_locals_dict_table);

const mp_obj_type_t box2d_b2ShapeDef_type = {
    {&mp_type_type},
    .name = MP_QSTR_b2ShapeDef,
    .print = box2d_b2ShapeDef_print,
    .make_new = box2d_b2ShapeDef_make_new,
    .locals_dict = (mp_obj_dict_t *)&box2d_b2ShapeDef_locals_dict,
};

mp_obj_t box2d_b2ShapeDef_make_new(const mp_obj_type_t *type,
    size_t n_args,
    size_t n_kw,
    const mp_obj_t *all_args) {

    enum {
        ARG_friction,
        ARG_restitution,
        ARG_density,
        ARG_isSensor,
    };
    static const mp_arg_t allowed_args[] = {
        {MP_QSTR_friction, MP_ARG_OBJ | MP_ARG_KW_ONLY, {.u_obj = NULL}},
        {MP_QSTR_restitution, MP_ARG_OBJ | MP_ARG_KW_ONLY, {.u_obj = NULL}},
        {MP_QSTR_density, MP_ARG_OBJ | MP_ARG_KW_ONLY, {.u_obj = NULL}},
        {MP_QSTR_isSensor, MP_ARG_INT | MP_ARG_KW_ONLY, {.u_int = 0}},
    };
    mp_arg_val_t args[MP_ARRAY_SIZE(allowed_args)];
    mp_arg_parse_all_kw_array(n_args, n_kw, all_args, MP_ARRAY_SIZE(allowed_args), allowed_args, args);

    box2d_b2ShapeDef_obj_t *self = m_new_obj(box2d_b2ShapeDef_obj_t);
    self->base.type = &box2d_b2ShapeDef_type;
    self->def = b2DefaultShapeDef();
    if(args[ARG_friction].u_obj != NULL) self->def.friction = mp_obj_get_float(args[ARG_friction].u_obj);
    if(args[ARG_restitution].u_obj != NULL) self->def.friction = mp_obj_get_float(args[ARG_restitution].u_obj);
    if(args[ARG_density].u_obj != NULL) self->def.friction = mp_obj_get_float(args[ARG_density].u_obj);
    self->def.isSensor = args[ARG_isSensor].u_int;
    return MP_OBJ_FROM_PTR(self);
}

mp_obj_t box2d_b2Shape_make_new(const mp_obj_type_t *type, size_t n_args, size_t n_kw, const mp_obj_t *args);

STATIC void box2d_b2Shape_print(const mp_print_t *print, mp_obj_t self_in, mp_print_kind_t kind) {
    (void)kind;
    box2d_b2Shape_obj_t *self = MP_OBJ_TO_PTR(self_in);
    mp_printf(print, "<b2Shape index=%u>", self->shape.index);
}

const mp_obj_type_t box2d_b2Body_type;
STATIC mp_obj_t box2d_b2Shape_GetBody(size_t n_args, const mp_obj_t *args) {
    box2d_b2Shape_obj_t *self = MP_OBJ_TO_PTR(args[0]);
    struct box2d_b2Body_obj_t* ret = m_new_obj(box2d_b2Body_obj_t);
    ret->base.type = &box2d_b2Body_type;
    ret->body = b2Shape_GetBody(self->shape);
    return MP_OBJ_FROM_PTR(ret);
}
STATIC MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(box2d_b2Shape_GetBody_obj, 1, 1, box2d_b2Shape_GetBody);

STATIC mp_obj_t box2d_b2Shape_TestPoint(size_t n_args, const mp_obj_t *args) {
    box2d_b2Shape_obj_t *self = MP_OBJ_TO_PTR(args[0]);
    if(b2Shape_TestPoint(self->shape, ((box2d_b2Vec2_obj_t*)MP_OBJ_FROM_PTR(args[1]))->vec)) return mp_const_true;
    else return mp_const_false;
}
STATIC MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(box2d_b2Shape_TestPoint_obj, 2, 2, box2d_b2Shape_TestPoint);

STATIC const mp_rom_map_elem_t box2d_b2Shape_locals_dict_table[] = {
  {MP_ROM_QSTR(MP_QSTR_GetBody), MP_ROM_PTR(&box2d_b2Shape_GetBody_obj)},
  {MP_ROM_QSTR(MP_QSTR_TestPoint), MP_ROM_PTR(&box2d_b2Shape_TestPoint_obj)},
};
STATIC MP_DEFINE_CONST_DICT(box2d_b2Shape_locals_dict, box2d_b2Shape_locals_dict_table);

const mp_obj_type_t box2d_b2Shape_type = {
    {&mp_type_type},
    .name = MP_QSTR_b2Shape,
    .print = box2d_b2Shape_print,
    .make_new = box2d_b2Shape_make_new,
    .locals_dict = (mp_obj_dict_t *)&box2d_b2Shape_locals_dict,
};

mp_obj_t box2d_b2Shape_make_new(const mp_obj_type_t *type,
    size_t n_args,
    size_t n_kw,
    const mp_obj_t *all_args) {

    enum {
        ARG_body,
        ARG_def,
        ARG_type,
    };
    static const mp_arg_t allowed_args[] = {
        {MP_QSTR_def, MP_ARG_OBJ | MP_ARG_REQUIRED, {.u_obj = NULL}},
        {MP_QSTR_body, MP_ARG_OBJ | MP_ARG_REQUIRED, {.u_obj = NULL}},
        {MP_QSTR_type, MP_ARG_INT | MP_ARG_REQUIRED, {.u_int = 0}},
    };
    mp_arg_val_t args[MP_ARRAY_SIZE(allowed_args)];
    mp_arg_parse_all_kw_array(n_args, n_kw, all_args, MP_ARRAY_SIZE(allowed_args), allowed_args, args);

    box2d_b2Shape_obj_t *self = m_new_obj(box2d_b2Shape_obj_t);
    self->base.type = &box2d_b2Shape_type;
    self->shape = b2_nullShapeId;
    return MP_OBJ_FROM_PTR(self);
}

mp_obj_t box2d_b2Polygon_make_new(const mp_obj_type_t *type, size_t n_args, size_t n_kw, const mp_obj_t *args);

STATIC void box2d_b2Polygon_print(const mp_print_t *print, mp_obj_t self_in, mp_print_kind_t kind) {
    (void)kind;
    box2d_b2Polygon_obj_t *self = MP_OBJ_TO_PTR(self_in);
    mp_printf(print, "<b2Polygon count=%u>", self->polygon.count);
}

STATIC mp_obj_t box2d_b2Polygon_MakeBox(size_t n_args, const mp_obj_t *args) {
    box2d_b2Polygon_obj_t *self = MP_OBJ_TO_PTR(args[0]);
    self->polygon = b2MakeBox(mp_obj_get_float(args[1]), mp_obj_get_float(args[2]));
    return mp_const_none;
}
STATIC MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(box2d_b2Polygon_MakeBox_obj, 3, 3, box2d_b2Polygon_MakeBox);

STATIC mp_obj_t box2d_b2Polygon_MakeRoundedBox(size_t n_args, const mp_obj_t *args) {
    box2d_b2Polygon_obj_t *self = MP_OBJ_TO_PTR(args[0]);
    self->polygon = b2MakeRoundedBox(mp_obj_get_float(args[1]), mp_obj_get_float(args[2]),mp_obj_get_float(args[3]));
    return mp_const_none;
}
STATIC MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(box2d_b2Polygon_MakeRoundedBox_obj, 4, 4, box2d_b2Polygon_MakeRoundedBox);

STATIC mp_obj_t box2d_b2Polygon_MakeCapsule(size_t n_args, const mp_obj_t *args) {
    box2d_b2Polygon_obj_t *self = MP_OBJ_TO_PTR(args[0]);
    self->polygon = b2MakeCapsule(((box2d_b2Vec2_obj_t *)MP_OBJ_TO_PTR(args[1]))->vec, ((box2d_b2Vec2_obj_t *)MP_OBJ_TO_PTR(args[2]))->vec, mp_obj_get_float(args[3]));
    return mp_const_none;
}
STATIC MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(box2d_b2Polygon_MakeCapsule_obj, 4, 4, box2d_b2Polygon_MakeCapsule);

STATIC const mp_rom_map_elem_t box2d_b2Polygon_locals_dict_table[] = {
  {MP_ROM_QSTR(MP_QSTR_MakeBox), MP_ROM_PTR(&box2d_b2Polygon_MakeBox_obj)},
  {MP_ROM_QSTR(MP_QSTR_MakeRoundedBox), MP_ROM_PTR(&box2d_b2Polygon_MakeRoundedBox_obj)},
  {MP_ROM_QSTR(MP_QSTR_MakeCapsule), MP_ROM_PTR(&box2d_b2Polygon_MakeCapsule_obj)},
};
STATIC MP_DEFINE_CONST_DICT(box2d_b2Polygon_locals_dict, box2d_b2Polygon_locals_dict_table);

const mp_obj_type_t box2d_b2Polygon_type = {
    {&mp_type_type},
    .name = MP_QSTR_b2Polygon,
    .print = box2d_b2Polygon_print,
    .make_new = box2d_b2Polygon_make_new,
    .locals_dict = (mp_obj_dict_t *)&box2d_b2Polygon_locals_dict,
};

mp_obj_t box2d_b2Polygon_make_new(const mp_obj_type_t *type,
    size_t n_args,
    size_t n_kw,
    const mp_obj_t *all_args) {

    enum {
        ARG_sidelen,
    };
    static const mp_arg_t allowed_args[] = {
        {MP_QSTR_sidelen, MP_ARG_OBJ | MP_ARG_KW_ONLY, {.u_obj = NULL}},
    };
    mp_arg_val_t args[MP_ARRAY_SIZE(allowed_args)];
    mp_arg_parse_all_kw_array(n_args, n_kw, all_args, MP_ARRAY_SIZE(allowed_args), allowed_args, args);

    box2d_b2Polygon_obj_t *self = m_new_obj(box2d_b2Polygon_obj_t);
    self->base.type = &box2d_b2Polygon_type;
    if(n_args <= 1) self->polygon = b2MakeBox(1.f, 1.f);
    else self->polygon = b2MakeBox(mp_obj_get_float(args[ARG_sidelen].u_obj), mp_obj_get_float(args[ARG_sidelen].u_obj));
    return MP_OBJ_FROM_PTR(self);
}
