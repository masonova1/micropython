#pragma once

mp_obj_t box2d_b2BodyDef_make_new(const mp_obj_type_t *type, size_t n_args, size_t n_kw, const mp_obj_t *args);

STATIC void box2d_b2BodyDef_print(const mp_print_t *print, mp_obj_t self_in, mp_print_kind_t kind) {
    (void)kind;
    box2d_b2BodyDef_obj_t *self = MP_OBJ_TO_PTR(self_in);
    mp_printf(print, "<b2BodyDef type=%u>", (int)self->def.type);
}

STATIC const mp_rom_map_elem_t box2d_b2BodyDef_locals_dict_table[] = {
  {MP_ROM_QSTR(MP_QSTR_write), MP_ROM_PTR(NULL)},
};
STATIC MP_DEFINE_CONST_DICT(box2d_b2BodyDef_locals_dict, box2d_b2BodyDef_locals_dict_table);

const mp_obj_type_t box2d_b2BodyDef_type = {
    {&mp_type_type},
    .name = MP_QSTR_b2BodyDef,
    .print = box2d_b2BodyDef_print,
    .make_new = box2d_b2BodyDef_make_new,
    .locals_dict = (mp_obj_dict_t *)&box2d_b2BodyDef_locals_dict,
};

mp_obj_t box2d_b2BodyDef_make_new(const mp_obj_type_t *type,
    size_t n_args,
    size_t n_kw,
    const mp_obj_t *all_args) {

    enum {
        ARG_type,
        ARG_position,
        ARG_angle,
        ARG_linearVelocity,
        ARG_linearDamping,
        ARG_angularVelocity,
        ARG_angularDamping,
        ARG_gravityScale,
        ARG_enableSleep,
        ARG_isAwake,
        ARG_fixedRotation,
        ARG_isEnabled,
    };
    static const mp_arg_t allowed_args[] = {
        {MP_QSTR_type, MP_ARG_INT | MP_ARG_KW_ONLY, {.u_int = b2_dynamicBody}},
        {MP_QSTR_position, MP_ARG_OBJ | MP_ARG_KW_ONLY, {.u_obj = NULL}},
        {MP_QSTR_angle, MP_ARG_OBJ | MP_ARG_KW_ONLY, {.u_obj = NULL}},
        {MP_QSTR_linearVelocity, MP_ARG_OBJ | MP_ARG_KW_ONLY, {.u_obj = NULL}},
        {MP_QSTR_linearDamping, MP_ARG_OBJ | MP_ARG_KW_ONLY, {.u_obj = NULL}},
        {MP_QSTR_angularVelocity, MP_ARG_OBJ | MP_ARG_KW_ONLY, {.u_obj = NULL}},
        {MP_QSTR_angularDamping, MP_ARG_OBJ | MP_ARG_KW_ONLY, {.u_obj = NULL}},
        {MP_QSTR_gravityScale, MP_ARG_OBJ | MP_ARG_KW_ONLY, {.u_obj = NULL}},
        {MP_QSTR_enableSleep, MP_ARG_INT | MP_ARG_KW_ONLY, {.u_int = 1}},
        {MP_QSTR_isAwake, MP_ARG_INT | MP_ARG_KW_ONLY, {.u_int = 1}},
        {MP_QSTR_fixedRotation, MP_ARG_INT | MP_ARG_KW_ONLY, {.u_int = 1}},
        {MP_QSTR_isEnabled, MP_ARG_INT | MP_ARG_KW_ONLY, {.u_int = 1}},
    };
    mp_arg_val_t args[MP_ARRAY_SIZE(allowed_args)];
    mp_arg_parse_all_kw_array(n_args, n_kw, all_args, MP_ARRAY_SIZE(allowed_args), allowed_args, args);

    box2d_b2BodyDef_obj_t *self = m_new_obj(box2d_b2BodyDef_obj_t);
    self->base.type = &box2d_b2BodyDef_type;
    self->def = b2DefaultBodyDef();
    self->def.type = args[ARG_type].u_int;
    if(args[ARG_position].u_obj != NULL) self->def.position = ((box2d_b2Vec2_obj_t *)MP_OBJ_TO_PTR(args[ARG_position].u_obj))->vec;
    if(args[ARG_angle].u_obj != NULL) self->def.angle = mp_obj_get_float(args[ARG_angle].u_obj);
    if(args[ARG_linearVelocity].u_obj != NULL) self->def.linearVelocity = ((box2d_b2Vec2_obj_t *)MP_OBJ_TO_PTR(args[ARG_linearVelocity].u_obj))->vec;
    if(args[ARG_linearDamping].u_obj != NULL) self->def.linearDamping = mp_obj_get_float(args[ARG_linearDamping].u_obj);
    if(args[ARG_angularVelocity].u_obj != NULL) self->def.angularVelocity = mp_obj_get_float(args[ARG_angularVelocity].u_obj);
    if(args[ARG_angularDamping].u_obj != NULL) self->def.angularDamping = mp_obj_get_float(args[ARG_angularDamping].u_obj);
    if(args[ARG_angularDamping].u_obj != NULL) self->def.gravityScale = mp_obj_get_float(args[ARG_gravityScale].u_obj);
    self->def.enableSleep = args[ARG_enableSleep].u_int;
    self->def.isAwake = args[ARG_isAwake].u_int;
    self->def.fixedRotation = args[ARG_fixedRotation].u_int;
    self->def.isEnabled = args[ARG_isEnabled].u_int;
    return MP_OBJ_FROM_PTR(self);
}

mp_obj_t box2d_b2Body_make_new(const mp_obj_type_t *type, size_t n_args, size_t n_kw, const mp_obj_t *args);

STATIC void box2d_b2Body_print(const mp_print_t *print, mp_obj_t self_in, mp_print_kind_t kind) {
    (void)kind;
    box2d_b2Body_obj_t *self = MP_OBJ_TO_PTR(self_in);
    mp_printf(print, "<b2Body index=%u>", (int)self->body.index);
}

STATIC mp_obj_t box2d_b2Body_GetPosition(size_t n_args, const mp_obj_t *args) {
    box2d_b2Body_obj_t *self = MP_OBJ_TO_PTR(args[0]);
    box2d_b2Vec2_obj_t* ret = m_new_obj(box2d_b2Vec2_obj_t);
    ret->base.type = &box2d_b2Vec2_type;
    ret->vec = b2Body_GetPosition(self->body);
    return MP_OBJ_FROM_PTR(ret);
}
STATIC MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(box2d_b2Body_GetPosition_obj, 1, 1, box2d_b2Body_GetPosition);

STATIC mp_obj_t box2d_b2Body_GetAngle(size_t n_args, const mp_obj_t *args) {
    box2d_b2Body_obj_t *self = MP_OBJ_TO_PTR(args[0]);
    return mp_obj_new_float(b2Body_GetAngle(self->body));
}
STATIC MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(box2d_b2Body_GetAngle_obj, 1, 1, box2d_b2Body_GetAngle);

STATIC mp_obj_t box2d_b2Body_GetMass(size_t n_args, const mp_obj_t *args) {
    box2d_b2Body_obj_t *self = MP_OBJ_TO_PTR(args[0]);
    return mp_obj_new_float(b2Body_GetMass(self->body));
}
STATIC MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(box2d_b2Body_GetMass_obj, 1, 1, box2d_b2Body_GetMass);

STATIC mp_obj_t box2d_b2Body_SetLinearVelocity(size_t n_args, const mp_obj_t *args) {
    box2d_b2Body_obj_t *self = MP_OBJ_TO_PTR(args[0]);
    b2Body_SetLinearVelocity(self->body, ((box2d_b2Vec2_obj_t*)MP_OBJ_FROM_PTR(args[1]))->vec);
    return mp_const_none;
}
STATIC MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(box2d_b2Body_SetLinearVelocity_obj, 2, 2, box2d_b2Body_SetLinearVelocity);

STATIC mp_obj_t box2d_b2Body_SetAngularVelocity(size_t n_args, const mp_obj_t *args) {
    box2d_b2Body_obj_t *self = MP_OBJ_TO_PTR(args[0]);
    b2Body_SetAngularVelocity(self->body, mp_obj_get_float(args[1]));
    return mp_const_none;
}
STATIC MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(box2d_b2Body_SetAngularVelocity_obj, 2, 2, box2d_b2Body_SetAngularVelocity);

STATIC mp_obj_t box2d_b2Body_CreatePolygon(size_t n_args, const mp_obj_t *args) {
    box2d_b2Body_obj_t *self = MP_OBJ_TO_PTR(args[0]);
    b2Body_CreatePolygon(self->body, &(((box2d_b2ShapeDef_obj_t*)MP_OBJ_TO_PTR(args[1]))->def), &(((box2d_b2Polygon_obj_t*)MP_OBJ_TO_PTR(args[2]))->polygon));
    return mp_const_none;
}
STATIC MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(box2d_b2Body_CreatePolygon_obj, 3, 3, box2d_b2Body_CreatePolygon);

STATIC const mp_rom_map_elem_t box2d_b2Body_locals_dict_table[] = {
  {MP_ROM_QSTR(MP_QSTR_GetPosition), MP_ROM_PTR(&box2d_b2Body_GetPosition_obj)},
  {MP_ROM_QSTR(MP_QSTR_GetAngle), MP_ROM_PTR(&box2d_b2Body_GetAngle_obj)},
  {MP_ROM_QSTR(MP_QSTR_GetMass), MP_ROM_PTR(&box2d_b2Body_GetMass_obj)},
  {MP_ROM_QSTR(MP_QSTR_SetLinearVelocity), MP_ROM_PTR(&box2d_b2Body_SetLinearVelocity_obj)},
  {MP_ROM_QSTR(MP_QSTR_SetAngularVelocity), MP_ROM_PTR(&box2d_b2Body_SetAngularVelocity_obj)},
  {MP_ROM_QSTR(MP_QSTR_CreatePolygon), MP_ROM_PTR(&box2d_b2Body_CreatePolygon_obj)},
};
STATIC MP_DEFINE_CONST_DICT(box2d_b2Body_locals_dict, box2d_b2Body_locals_dict_table);

const mp_obj_type_t box2d_b2Body_type = {
    {&mp_type_type},
    .name = MP_QSTR_b2Body,
    .print = box2d_b2Body_print,
    .make_new = box2d_b2Body_make_new,
    .locals_dict = (mp_obj_dict_t *)&box2d_b2Body_locals_dict,
};

mp_obj_t box2d_b2Body_make_new(const mp_obj_type_t *type,
    size_t n_args,
    size_t n_kw,
    const mp_obj_t *all_args) {

    enum {
        ARG_world,
        ARG_def,
    };
    static const mp_arg_t allowed_args[] = {
        {MP_QSTR_world, MP_ARG_OBJ | MP_ARG_REQUIRED, {.u_obj = MP_OBJ_NULL}},
        {MP_QSTR_def, MP_ARG_OBJ | MP_ARG_REQUIRED, {.u_obj = MP_OBJ_NULL}},
    };
    mp_arg_val_t args[MP_ARRAY_SIZE(allowed_args)];
    mp_arg_parse_all_kw_array(n_args, n_kw, all_args, MP_ARRAY_SIZE(allowed_args), allowed_args, args);

    box2d_b2Body_obj_t *self = m_new_obj(box2d_b2Body_obj_t);
    box2d_b2World_obj_t* world_obj = (box2d_b2World_obj_t *)MP_OBJ_TO_PTR(args[ARG_world].u_obj);
    box2d_b2BodyDef_obj_t* def_obj = (box2d_b2BodyDef_obj_t *)MP_OBJ_TO_PTR(args[ARG_def].u_obj);
    self->base.type = &box2d_b2Body_type;
    self->body = b2World_CreateBody(world_obj->world, &def_obj->def);
    return MP_OBJ_FROM_PTR(self);
}
