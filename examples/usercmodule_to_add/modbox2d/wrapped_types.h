#pragma once

mp_obj_t box2d_b2Vec2_make_new(const mp_obj_type_t *type, size_t n_args, size_t n_kw, const mp_obj_t *args);

STATIC void box2d_b2Vec2_print(const mp_print_t *print, mp_obj_t self_in, mp_print_kind_t kind) {
    (void)kind;
    box2d_b2Vec2_obj_t *self = MP_OBJ_TO_PTR(self_in);
    mp_printf(print, "<b2Vec2 x=%f, y=%f>", self->vec.x, self->vec.y);
}

STATIC mp_obj_t box2d_b2Vec2_tup(size_t n_args, const mp_obj_t *args) {
    box2d_b2Vec2_obj_t *self = MP_OBJ_TO_PTR(args[0]);
    mp_obj_t ret[2];
    ret[0] = mp_obj_new_float(self->vec.x);
    ret[1] = mp_obj_new_float(self->vec.y);
    return mp_obj_new_tuple(2, ret);
}
STATIC MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(box2d_b2Vec2_tup_obj, 1, 1, box2d_b2Vec2_tup);

STATIC mp_obj_t box2d_b2Vec2_x(size_t n_args, const mp_obj_t *args) {
    box2d_b2Vec2_obj_t *self = MP_OBJ_TO_PTR(args[0]);
    if(n_args >= 2) self->vec.x = mp_obj_get_float(args[1]);
    return mp_obj_new_float(self->vec.x);
}
STATIC MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(box2d_b2Vec2_x_obj, 1, 2, box2d_b2Vec2_x);

STATIC mp_obj_t box2d_b2Vec2_y(size_t n_args, const mp_obj_t *args) {
    box2d_b2Vec2_obj_t *self = MP_OBJ_TO_PTR(args[0]);
    if(n_args >= 2) self->vec.y = mp_obj_get_float(args[1]);
    return mp_obj_new_float(self->vec.y);
}
STATIC MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(box2d_b2Vec2_y_obj, 1, 2, box2d_b2Vec2_y);

STATIC const mp_rom_map_elem_t box2d_b2Vec2_locals_dict_table[] = {
  {MP_ROM_QSTR(MP_QSTR_tup), MP_ROM_PTR(&box2d_b2Vec2_tup_obj)},
  {MP_ROM_QSTR(MP_QSTR_x), MP_ROM_PTR(&box2d_b2Vec2_x_obj)},
  {MP_ROM_QSTR(MP_QSTR_y), MP_ROM_PTR(&box2d_b2Vec2_y_obj)},
};
STATIC MP_DEFINE_CONST_DICT(box2d_b2Vec2_locals_dict, box2d_b2Vec2_locals_dict_table);

const mp_obj_type_t box2d_b2Vec2_type = {
    {&mp_type_type},
    .name = MP_QSTR_b2Vec2,
    .print = box2d_b2Vec2_print,
    .make_new = box2d_b2Vec2_make_new,
    .locals_dict = (mp_obj_dict_t *)&box2d_b2Vec2_locals_dict,
};

mp_obj_t box2d_b2Vec2_make_new(const mp_obj_type_t *type,
    size_t n_args,
    size_t n_kw,
    const mp_obj_t *all_args) {

    enum {
        ARG_xv,
        ARG_yv,
    };
    static const mp_arg_t allowed_args[] = {
        {MP_QSTR_xv, MP_ARG_OBJ | MP_ARG_REQUIRED, {.u_obj = MP_OBJ_NULL}},
        {MP_QSTR_yv, MP_ARG_OBJ | MP_ARG_REQUIRED, {.u_obj = MP_OBJ_NULL}},
    };
    mp_arg_val_t args[MP_ARRAY_SIZE(allowed_args)];
    mp_arg_parse_all_kw_array(n_args, n_kw, all_args, MP_ARRAY_SIZE(allowed_args), allowed_args, args);

    const float x = mp_obj_get_float(MP_OBJ_TO_PTR(args[ARG_xv].u_obj));
    const float y = mp_obj_get_float(MP_OBJ_TO_PTR(args[ARG_yv].u_obj));

    box2d_b2Vec2_obj_t *self = m_new_obj(box2d_b2Vec2_obj_t);
    self->base.type = &box2d_b2Vec2_type;
    self->vec = (b2Vec2){x, y};
    return MP_OBJ_FROM_PTR(self);
}

mp_obj_t box2d_b2Rot_make_new(const mp_obj_type_t *type, size_t n_args, size_t n_kw, const mp_obj_t *args);

STATIC void box2d_b2Rot_print(const mp_print_t *print, mp_obj_t self_in, mp_print_kind_t kind) {
    (void)kind;
    box2d_b2Rot_obj_t *self = MP_OBJ_TO_PTR(self_in);
    mp_printf(print, "<b2Rot s=%f, c=%f>", self->rot.s, self->rot.c);
}

STATIC mp_obj_t box2d_b2Rot_tup(size_t n_args, const mp_obj_t *args) {
    box2d_b2Rot_obj_t *self = MP_OBJ_TO_PTR(args[0]);
    mp_obj_t ret[2];
    ret[0] = mp_obj_new_float(self->rot.s);
    ret[1] = mp_obj_new_float(self->rot.c);
    return mp_obj_new_tuple(2, ret);
}
STATIC MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(box2d_b2Rot_tup_obj, 1, 1, box2d_b2Rot_tup);

STATIC mp_obj_t box2d_b2Rot_s(size_t n_args, const mp_obj_t *args) {
    box2d_b2Rot_obj_t *self = MP_OBJ_TO_PTR(args[0]);
    if(n_args >= 2) self->rot.s = mp_obj_get_float(args[1]);
    return mp_obj_new_float(self->rot.s);
}
STATIC MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(box2d_b2Rot_s_obj, 1, 2, box2d_b2Rot_s);

STATIC mp_obj_t box2d_b2Rot_c(size_t n_args, const mp_obj_t *args) {
  box2d_b2Rot_obj_t *self = MP_OBJ_TO_PTR(args[0]);
  if(n_args >= 2) self->rot.c = mp_obj_get_float(args[1]);
  return mp_obj_new_float(self->rot.c);
}
STATIC MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(box2d_b2Rot_c_obj, 1, 2, box2d_b2Rot_c);

STATIC const mp_rom_map_elem_t box2d_b2Rot_locals_dict_table[] = {
  {MP_ROM_QSTR(MP_QSTR_tup), MP_ROM_PTR(&box2d_b2Rot_tup_obj)},
  {MP_ROM_QSTR(MP_QSTR_s), MP_ROM_PTR(&box2d_b2Rot_s_obj)},
  {MP_ROM_QSTR(MP_QSTR_c), MP_ROM_PTR(&box2d_b2Rot_c_obj)},
};
STATIC MP_DEFINE_CONST_DICT(box2d_b2Rot_locals_dict, box2d_b2Rot_locals_dict_table);

const mp_obj_type_t box2d_b2Rot_type = {
    {&mp_type_type},
    .name = MP_QSTR_b2Rot,
    .print = box2d_b2Rot_print,
    .make_new = box2d_b2Rot_make_new,
    .locals_dict = (mp_obj_dict_t *)&box2d_b2Rot_locals_dict,
};

mp_obj_t box2d_b2Rot_make_new(const mp_obj_type_t *type,
    size_t n_args,
    size_t n_kw,
    const mp_obj_t *all_args) {

    enum {
        ARG_sv,
        ARG_cv,
    };
    static const mp_arg_t allowed_args[] = {
        {MP_QSTR_xv, MP_ARG_OBJ | MP_ARG_REQUIRED, {.u_obj = MP_OBJ_NULL}},
        {MP_QSTR_yv, MP_ARG_OBJ | MP_ARG_REQUIRED, {.u_obj = MP_OBJ_NULL}},
    };
    mp_arg_val_t args[MP_ARRAY_SIZE(allowed_args)];
    mp_arg_parse_all_kw_array(n_args, n_kw, all_args, MP_ARRAY_SIZE(allowed_args), allowed_args, args);

    const float s = mp_obj_get_float(MP_OBJ_TO_PTR(args[ARG_sv].u_obj));
    const float c = mp_obj_get_float(MP_OBJ_TO_PTR(args[ARG_cv].u_obj));

    box2d_b2Rot_obj_t *self = m_new_obj(box2d_b2Rot_obj_t);
    self->base.type = &box2d_b2Rot_type;
    self->rot = (b2Rot){s, c};
    return MP_OBJ_FROM_PTR(self);
}

mp_obj_t box2d_b2Transform_make_new(const mp_obj_type_t *type, size_t n_args, size_t n_kw, const mp_obj_t *args);

STATIC void box2d_b2Transform_print(const mp_print_t *print, mp_obj_t self_in, mp_print_kind_t kind) {
    (void)kind;
    box2d_b2Transform_obj_t *self = MP_OBJ_TO_PTR(self_in);
    mp_printf(print, "<b2Transform p={%f, %f}, q={%f, %f}>", self->transform.p.x, self->transform.p.y, self->transform.q.s, self->transform.q.c);
}

STATIC mp_obj_t box2d_b2Transform_p(size_t n_args, const mp_obj_t *args) {
    box2d_b2Transform_obj_t *self = MP_OBJ_TO_PTR(args[0]);
    if(n_args >= 2) self->transform.p = ((box2d_b2Vec2_obj_t*)MP_OBJ_FROM_PTR(args[1]))->vec;
    box2d_b2Vec2_obj_t* ret = m_new_obj(box2d_b2Vec2_obj_t);
    ret->base.type = &box2d_b2Vec2_type;
    ret->vec = self->transform.p;
    return MP_OBJ_FROM_PTR(ret);
}
STATIC MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(box2d_b2Transform_p_obj, 1, 2, box2d_b2Transform_p);

STATIC mp_obj_t box2d_b2Transform_q(size_t n_args, const mp_obj_t *args) {
    box2d_b2Transform_obj_t *self = MP_OBJ_TO_PTR(args[0]);
    if(n_args >= 2) self->transform.q = ((box2d_b2Rot_obj_t*)MP_OBJ_FROM_PTR(args[1]))->rot;
    box2d_b2Rot_obj_t* ret = m_new_obj(box2d_b2Rot_obj_t);
    ret->base.type = &box2d_b2Rot_type;
    ret->rot = self->transform.q;
    return MP_OBJ_FROM_PTR(ret);
}
STATIC MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(box2d_b2Transform_q_obj, 1, 2, box2d_b2Transform_q);

STATIC const mp_rom_map_elem_t box2d_b2Transform_locals_dict_table[] = {
  {MP_ROM_QSTR(MP_QSTR_p), MP_ROM_PTR(&box2d_b2Transform_p_obj)},
  {MP_ROM_QSTR(MP_QSTR_q), MP_ROM_PTR(&box2d_b2Transform_q_obj)},
};
STATIC MP_DEFINE_CONST_DICT(box2d_b2Transform_locals_dict, box2d_b2Transform_locals_dict_table);

const mp_obj_type_t box2d_b2Transform_type = {
    {&mp_type_type},
    .name = MP_QSTR_b2Transform,
    .print = box2d_b2Transform_print,
    .make_new = box2d_b2Transform_make_new,
    .locals_dict = (mp_obj_dict_t *)&box2d_b2Transform_locals_dict,
};

mp_obj_t box2d_b2Transform_make_new(const mp_obj_type_t *type,
    size_t n_args,
    size_t n_kw,
    const mp_obj_t *all_args) {

    enum {
        ARG_p,
        ARG_q,
    };
    static const mp_arg_t allowed_args[] = {
        {MP_QSTR_p, MP_ARG_OBJ | MP_ARG_REQUIRED, {.u_obj = MP_OBJ_NULL}},
        {MP_QSTR_q, MP_ARG_OBJ | MP_ARG_REQUIRED, {.u_obj = MP_OBJ_NULL}},
    };
    mp_arg_val_t args[MP_ARRAY_SIZE(allowed_args)];
    mp_arg_parse_all_kw_array(n_args, n_kw, all_args, MP_ARRAY_SIZE(allowed_args), allowed_args, args);
    box2d_b2Vec2_obj_t* p_obj = (box2d_b2Vec2_obj_t *)MP_OBJ_TO_PTR(args[ARG_p].u_obj);
    box2d_b2Rot_obj_t* q_obj = (box2d_b2Rot_obj_t *)MP_OBJ_TO_PTR(args[ARG_q].u_obj);
    box2d_b2Transform_obj_t *self = m_new_obj(box2d_b2Transform_obj_t);
    self->base.type = &box2d_b2Transform_type;
    self->transform.p = p_obj->vec;
    self->transform.q = q_obj->rot;
    return MP_OBJ_FROM_PTR(self);
}

mp_obj_t box2d_b2Mat22_make_new(const mp_obj_type_t *type, size_t n_args, size_t n_kw, const mp_obj_t *args);

STATIC void box2d_b2Mat22_print(const mp_print_t *print, mp_obj_t self_in, mp_print_kind_t kind) {
    (void)kind;
    box2d_b2Mat22_obj_t *self = MP_OBJ_TO_PTR(self_in);
    mp_printf(print, "<b2Mat22 cx={%f, %f}, cy={%f, %f}>", self->mat.cx.x, self->mat.cx.y, self->mat.cy.x, self->mat.cy.y);
}

STATIC mp_obj_t box2d_b2Mat22_cx(size_t n_args, const mp_obj_t *args) {
    box2d_b2Mat22_obj_t *self = MP_OBJ_TO_PTR(args[0]);
    if(n_args >= 2) self->mat.cx = ((box2d_b2Vec2_obj_t*)MP_OBJ_FROM_PTR(args[1]))->vec;
    box2d_b2Vec2_obj_t* ret = m_new_obj(box2d_b2Vec2_obj_t);
    ret->base.type = &box2d_b2Vec2_type;
    ret->vec = self->mat.cx;
    return MP_OBJ_FROM_PTR(ret);
}
STATIC MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(box2d_b2Mat22_cx_obj, 1, 2, box2d_b2Mat22_cx);

STATIC mp_obj_t box2d_b2Mat22_cy(size_t n_args, const mp_obj_t *args) {
    box2d_b2Mat22_obj_t *self = MP_OBJ_TO_PTR(args[0]);
    if(n_args >= 2) self->mat.cy = ((box2d_b2Vec2_obj_t*)MP_OBJ_FROM_PTR(args[1]))->vec;
    box2d_b2Vec2_obj_t* ret = m_new_obj(box2d_b2Vec2_obj_t);
    ret->base.type = &box2d_b2Vec2_type;
    ret->vec = self->mat.cy;
    return MP_OBJ_FROM_PTR(ret);
}
STATIC MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(box2d_b2Mat22_cy_obj, 1, 2, box2d_b2Mat22_cy);

STATIC const mp_rom_map_elem_t box2d_b2Mat22_locals_dict_table[] = {
  {MP_ROM_QSTR(MP_QSTR_cx), MP_ROM_PTR(&box2d_b2Mat22_cx_obj)},
  {MP_ROM_QSTR(MP_QSTR_cy), MP_ROM_PTR(&box2d_b2Mat22_cy_obj)},
};
STATIC MP_DEFINE_CONST_DICT(box2d_b2Mat22_locals_dict, box2d_b2Mat22_locals_dict_table);

const mp_obj_type_t box2d_b2Mat22_type = {
    {&mp_type_type},
    .name = MP_QSTR_b2Mat22,
    .print = box2d_b2Mat22_print,
    .make_new = box2d_b2Mat22_make_new,
    .locals_dict = (mp_obj_dict_t *)&box2d_b2Mat22_locals_dict,
};

mp_obj_t box2d_b2Mat22_make_new(const mp_obj_type_t *type,
    size_t n_args,
    size_t n_kw,
    const mp_obj_t *all_args) {

    enum {
        ARG_cx,
        ARG_cy,
    };
    static const mp_arg_t allowed_args[] = {
        {MP_QSTR_cx, MP_ARG_OBJ | MP_ARG_REQUIRED, {.u_obj = MP_OBJ_NULL}},
        {MP_QSTR_cy, MP_ARG_OBJ | MP_ARG_REQUIRED, {.u_obj = MP_OBJ_NULL}},
    };
    mp_arg_val_t args[MP_ARRAY_SIZE(allowed_args)];
    mp_arg_parse_all_kw_array(n_args, n_kw, all_args, MP_ARRAY_SIZE(allowed_args), allowed_args, args);
    box2d_b2Vec2_obj_t* cx_obj = (box2d_b2Vec2_obj_t *)MP_OBJ_TO_PTR(args[ARG_cx].u_obj);
    box2d_b2Vec2_obj_t* cy_obj = (box2d_b2Vec2_obj_t *)MP_OBJ_TO_PTR(args[ARG_cy].u_obj);
    box2d_b2Mat22_obj_t *self = m_new_obj(box2d_b2Mat22_obj_t);
    self->base.type = &box2d_b2Mat22_type;
    self->mat = (b2Mat22){cx_obj->vec, cy_obj->vec};
    return MP_OBJ_FROM_PTR(self);
}

mp_obj_t box2d_b2AABB_make_new(const mp_obj_type_t *type, size_t n_args, size_t n_kw, const mp_obj_t *args);

STATIC void box2d_b2AABB_print(const mp_print_t *print, mp_obj_t self_in, mp_print_kind_t kind) {
    (void)kind;
    box2d_b2AABB_obj_t *self = MP_OBJ_TO_PTR(self_in);
    mp_printf(print, "<b2AABB lb={%f, %f}, ub={%f, %f}>", self->aabb.lowerBound.x, self->aabb.lowerBound.y, self->aabb.upperBound.x, self->aabb.upperBound.y);
}

STATIC mp_obj_t box2d_b2AABB_lowerBound(size_t n_args, const mp_obj_t *args) {
    box2d_b2AABB_obj_t *self = MP_OBJ_TO_PTR(args[0]);
    if(n_args >= 2) self->aabb.lowerBound = ((box2d_b2Vec2_obj_t*)MP_OBJ_FROM_PTR(args[1]))->vec;
    box2d_b2Vec2_obj_t* ret = m_new_obj(box2d_b2Vec2_obj_t);
    ret->base.type = &box2d_b2Vec2_type;
    ret->vec = self->aabb.lowerBound;
    return MP_OBJ_FROM_PTR(ret);
}
STATIC MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(box2d_b2AABB_lowerBound_obj, 1, 2, box2d_b2AABB_lowerBound);

STATIC mp_obj_t box2d_b2AABB_upperBound(size_t n_args, const mp_obj_t *args) {
    box2d_b2AABB_obj_t *self = MP_OBJ_TO_PTR(args[0]);
    if(n_args >= 2) self->aabb.upperBound = ((box2d_b2Vec2_obj_t*)MP_OBJ_FROM_PTR(args[1]))->vec;
    box2d_b2Vec2_obj_t* ret = m_new_obj(box2d_b2Vec2_obj_t);
    ret->base.type = &box2d_b2Vec2_type;
    ret->vec = self->aabb.upperBound;
    return MP_OBJ_FROM_PTR(ret);
}
STATIC MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(box2d_b2AABB_upperBound_obj, 1, 2, box2d_b2AABB_upperBound);

STATIC const mp_rom_map_elem_t box2d_b2AABB_locals_dict_table[] = {
  {MP_ROM_QSTR(MP_QSTR_lowerBound), MP_ROM_PTR(&box2d_b2AABB_lowerBound_obj)},
  {MP_ROM_QSTR(MP_QSTR_upperBound), MP_ROM_PTR(&box2d_b2AABB_upperBound_obj)},
};
STATIC MP_DEFINE_CONST_DICT(box2d_b2AABB_locals_dict, box2d_b2AABB_locals_dict_table);

const mp_obj_type_t box2d_b2AABB_type = {
    {&mp_type_type},
    .name = MP_QSTR_b2AABB,
    .print = box2d_b2AABB_print,
    .make_new = box2d_b2AABB_make_new,
    .locals_dict = (mp_obj_dict_t *)&box2d_b2AABB_locals_dict,
};

mp_obj_t box2d_b2AABB_make_new(const mp_obj_type_t *type,
    size_t n_args,
    size_t n_kw,
    const mp_obj_t *all_args) {

    enum {
        ARG_lb,
        ARG_ub,
    };
    static const mp_arg_t allowed_args[] = {
        {MP_QSTR_lb, MP_ARG_OBJ | MP_ARG_REQUIRED, {.u_obj = MP_OBJ_NULL}},
        {MP_QSTR_ub, MP_ARG_OBJ | MP_ARG_REQUIRED, {.u_obj = MP_OBJ_NULL}},
    };
    mp_arg_val_t args[MP_ARRAY_SIZE(allowed_args)];
    mp_arg_parse_all_kw_array(n_args, n_kw, all_args, MP_ARRAY_SIZE(allowed_args), allowed_args, args);
    box2d_b2Vec2_obj_t* lb_obj = (box2d_b2Vec2_obj_t *)MP_OBJ_TO_PTR(args[ARG_lb].u_obj);
    box2d_b2Vec2_obj_t* ub_obj = (box2d_b2Vec2_obj_t *)MP_OBJ_TO_PTR(args[ARG_ub].u_obj);
    box2d_b2AABB_obj_t *self = m_new_obj(box2d_b2AABB_obj_t);
    self->base.type = &box2d_b2AABB_type;
    self->aabb = (b2AABB){lb_obj->vec, ub_obj->vec};
    return MP_OBJ_FROM_PTR(self);
}

mp_obj_t box2d_b2RayCastInput_make_new(const mp_obj_type_t *type, size_t n_args, size_t n_kw, const mp_obj_t *args);

STATIC void box2d_b2RayCastInput_print(const mp_print_t *print, mp_obj_t self_in, mp_print_kind_t kind) {
    (void)kind;
    box2d_b2RayCastInput_obj_t *self = MP_OBJ_TO_PTR(self_in);
    mp_printf(print, "<b2RayCastInput p1={%f, %f}, p2={%f, %f}, radius=%f, maxFraction=%f>", self->rayCastInput.p1.x, self->rayCastInput.p1.y, self->rayCastInput.p2.x, self->rayCastInput.p2.y, self->rayCastInput.radius, self->rayCastInput.maxFraction);
}

STATIC const mp_rom_map_elem_t box2d_b2RayCastInput_locals_dict_table[] = {
  {MP_ROM_QSTR(MP_QSTR_write), MP_ROM_PTR(NULL)},
};
STATIC MP_DEFINE_CONST_DICT(box2d_b2RayCastInput_locals_dict, box2d_b2RayCastInput_locals_dict_table);

const mp_obj_type_t box2d_b2RayCastInput_type = {
    {&mp_type_type},
    .name = MP_QSTR_b2RayCastInput,
    .print = box2d_b2RayCastInput_print,
    .make_new = box2d_b2RayCastInput_make_new,
    .locals_dict = (mp_obj_dict_t *)&box2d_b2RayCastInput_locals_dict,
};

mp_obj_t box2d_b2RayCastInput_make_new(const mp_obj_type_t *type,
    size_t n_args,
    size_t n_kw,
    const mp_obj_t *all_args) {

    enum {
        ARG_p1,
        ARG_p2,
        ARG_radius,
        ARG_maxFraction,
    };
    static const mp_arg_t allowed_args[] = {
        {MP_QSTR_p1, MP_ARG_OBJ | MP_ARG_REQUIRED, {.u_obj = MP_OBJ_NULL}},
        {MP_QSTR_p2, MP_ARG_OBJ | MP_ARG_REQUIRED, {.u_obj = MP_OBJ_NULL}},
        {MP_QSTR_radius, MP_ARG_OBJ | MP_ARG_REQUIRED, {.u_obj = MP_OBJ_NULL}},
        {MP_QSTR_maxFraction, MP_ARG_OBJ | MP_ARG_REQUIRED, {.u_obj = MP_OBJ_NULL}},
    };
    mp_arg_val_t args[MP_ARRAY_SIZE(allowed_args)];
    mp_arg_parse_all_kw_array(n_args, n_kw, all_args, MP_ARRAY_SIZE(allowed_args), allowed_args, args);
    box2d_b2Vec2_obj_t* p1_obj = (box2d_b2Vec2_obj_t *)MP_OBJ_TO_PTR(args[ARG_p1].u_obj);
    box2d_b2Vec2_obj_t* p2_obj = (box2d_b2Vec2_obj_t *)MP_OBJ_TO_PTR(args[ARG_p2].u_obj);
    const float radius = mp_obj_get_float(MP_OBJ_TO_PTR(args[ARG_radius].u_obj));
    const float maxFraction = mp_obj_get_float(MP_OBJ_TO_PTR(args[ARG_maxFraction].u_obj));
    box2d_b2RayCastInput_obj_t *self = m_new_obj(box2d_b2RayCastInput_obj_t);
    self->base.type = &box2d_b2RayCastInput_type;
    self->rayCastInput = (b2RayCastInput){p1_obj->vec, p2_obj->vec, radius, maxFraction};
    return MP_OBJ_FROM_PTR(self);
}

mp_obj_t box2d_b2RayCastOutput_make_new(const mp_obj_type_t *type, size_t n_args, size_t n_kw, const mp_obj_t *args);

STATIC void box2d_b2RayCastOutput_print(const mp_print_t *print, mp_obj_t self_in, mp_print_kind_t kind) {
    (void)kind;
    box2d_b2RayCastOutput_obj_t *self = MP_OBJ_TO_PTR(self_in);
    mp_printf(print, "<b2RayCastOutput normal={%f, %f}, point={%f, %f}, fraction=%f, iterations=%i, hit=%i>", self->rayCastOutput.normal.x, self->rayCastOutput.normal.y, self->rayCastOutput.point.x, self->rayCastOutput.point.y, self->rayCastOutput.fraction, (int)self->rayCastOutput.iterations, (int)self->rayCastOutput.hit);
}

STATIC const mp_rom_map_elem_t box2d_b2RayCastOutput_locals_dict_table[] = {
  {MP_ROM_QSTR(MP_QSTR_write), MP_ROM_PTR(NULL)},
};
STATIC MP_DEFINE_CONST_DICT(box2d_b2RayCastOutput_locals_dict, box2d_b2RayCastOutput_locals_dict_table);

const mp_obj_type_t box2d_b2RayCastOutput_type = {
    {&mp_type_type},
    .name = MP_QSTR_b2RayCastOutput,
    .print = box2d_b2RayCastOutput_print,
    .make_new = box2d_b2RayCastOutput_make_new,
    .locals_dict = (mp_obj_dict_t *)&box2d_b2RayCastOutput_locals_dict,
};

mp_obj_t box2d_b2RayCastOutput_make_new(const mp_obj_type_t *type,
    size_t n_args,
    size_t n_kw,
    const mp_obj_t *all_args) {

    enum {
        ARG_normal,
        ARG_point,
        ARG_fraction,
        ARG_iterations,
        ARG_hit,
    };
    static const mp_arg_t allowed_args[] = {
        {MP_QSTR_normal, MP_ARG_OBJ | MP_ARG_REQUIRED, {.u_obj = MP_OBJ_NULL}},
        {MP_QSTR_point, MP_ARG_OBJ | MP_ARG_REQUIRED, {.u_obj = MP_OBJ_NULL}},
        {MP_QSTR_fraction, MP_ARG_OBJ | MP_ARG_REQUIRED, {.u_obj = MP_OBJ_NULL}},
        {MP_QSTR_iterations, MP_ARG_INT | MP_ARG_REQUIRED, {.u_obj = MP_OBJ_NULL}},
        {MP_QSTR_hit, MP_ARG_INT | MP_ARG_REQUIRED, {.u_obj = MP_OBJ_NULL}},
    };
    mp_arg_val_t args[MP_ARRAY_SIZE(allowed_args)];
    mp_arg_parse_all_kw_array(n_args, n_kw, all_args, MP_ARRAY_SIZE(allowed_args), allowed_args, args);
    box2d_b2Vec2_obj_t* normal_obj = (box2d_b2Vec2_obj_t *)MP_OBJ_TO_PTR(args[ARG_normal].u_obj);
    box2d_b2Vec2_obj_t* point_obj = (box2d_b2Vec2_obj_t *)MP_OBJ_TO_PTR(args[ARG_point].u_obj);
    const float fraction = mp_obj_get_float(MP_OBJ_TO_PTR(args[ARG_fraction].u_obj));
    const int iterations = args[ARG_iterations].u_int;
    const int hit = args[ARG_hit].u_int;
    box2d_b2RayCastOutput_obj_t *self = m_new_obj(box2d_b2RayCastOutput_obj_t);
    self->base.type = &box2d_b2RayCastOutput_type;
    self->rayCastOutput = (b2RayCastOutput){normal_obj->vec, point_obj->vec, fraction, iterations, hit};
    return MP_OBJ_FROM_PTR(self);
}

mp_obj_t box2d_b2Filter_make_new(const mp_obj_type_t *type, size_t n_args, size_t n_kw, const mp_obj_t *args);

STATIC void box2d_b2Filter_print(const mp_print_t *print, mp_obj_t self_in, mp_print_kind_t kind) {
    (void)kind;
    box2d_b2Filter_obj_t *self = MP_OBJ_TO_PTR(self_in);
    mp_printf(print, "<b2Filter fcategory=%u, mask=%u, group=%u>", self->filter.categoryBits, self->filter.maskBits, self->filter.groupIndex);
}

STATIC const mp_rom_map_elem_t box2d_b2Filter_locals_dict_table[] = {
  {MP_ROM_QSTR(MP_QSTR_write), MP_ROM_PTR(NULL)},
};
STATIC MP_DEFINE_CONST_DICT(box2d_b2Filter_locals_dict, box2d_b2Filter_locals_dict_table);

const mp_obj_type_t box2d_b2Filter_type = {
    {&mp_type_type},
    .name = MP_QSTR_b2Filter,
    .print = box2d_b2Filter_print,
    .make_new = box2d_b2Filter_make_new,
    .locals_dict = (mp_obj_dict_t *)&box2d_b2Filter_locals_dict,
};

mp_obj_t box2d_b2Filter_make_new(const mp_obj_type_t *type,
    size_t n_args,
    size_t n_kw,
    const mp_obj_t *all_args) {

    enum {
        ARG_categoryBits,
        ARG_maskBits,
        ARG_groupIndex,
    };
    static const mp_arg_t allowed_args[] = {
        {MP_QSTR_categoryBits, MP_ARG_INT | MP_ARG_KW_ONLY, {.u_int = 0}},
        {MP_QSTR_maskBits, MP_ARG_INT | MP_ARG_KW_ONLY, {.u_int = 0}},
        {MP_QSTR_groupIndex, MP_ARG_INT | MP_ARG_KW_ONLY, {.u_int = 0}},
    };
    mp_arg_val_t args[MP_ARRAY_SIZE(allowed_args)];
    mp_arg_parse_all_kw_array(n_args, n_kw, all_args, MP_ARRAY_SIZE(allowed_args), allowed_args, args);

    box2d_b2Filter_obj_t *self = m_new_obj(box2d_b2Filter_obj_t);
    self->base.type = &box2d_b2Filter_type;
    self->filter.categoryBits = args[ARG_categoryBits].u_int;
    self->filter.maskBits = args[ARG_maskBits].u_int;
    self->filter.groupIndex = args[ARG_groupIndex].u_int;
    return MP_OBJ_FROM_PTR(self);
}
