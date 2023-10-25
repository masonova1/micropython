#include "py/builtin.h"
#include "py/runtime.h"
#include "py/mpstate.h"
#include "py/obj.h"
#include "py/objstr.h"
#include "py/objmodule.h"
#include "include/box2d/box2d.h"
#include "include/box2d/constants.h"
#include "include/box2d/geometry.h"

static b2WorldDef worldDef;
static b2WorldId test;

typedef struct box2d_b2Vec2_obj_t
{
  mp_obj_base_t base;
  b2Vec2 vec;
} box2d_b2Vec2_obj_t;

typedef struct box2d_b2Rot_obj_t
{
  mp_obj_base_t base;
  b2Rot rot;
} box2d_b2Rot_obj_t;

typedef struct box2d_b2Transform_obj_t
{
  mp_obj_base_t base;
  b2Transform transform;
} box2d_b2Transform_obj_t;

typedef struct box2d_b2Mat22_obj_t
{
  mp_obj_base_t base;
  b2Mat22 mat;
} box2d_b2Mat22_obj_t;

typedef struct box2d_b2AABB_obj_t
{
  mp_obj_base_t base;
  b2AABB aabb;
} box2d_b2AABB_obj_t;

typedef struct box2d_b2RayCastInput_obj_t
{
  mp_obj_base_t base;
  b2RayCastInput rayCastInput;
} box2d_b2RayCastInput_obj_t;

typedef struct box2d_b2RayCastOutput_obj_t
{
  mp_obj_base_t base;
  b2RayCastOutput rayCastOutput;
} box2d_b2RayCastOutput_obj_t;

typedef struct box2d_b2Filter_obj_t
{
  mp_obj_base_t base;
  b2Filter filter;
} box2d_b2Filter_obj_t;

typedef struct box2d_b2ShapeDef_obj_t
{
  mp_obj_base_t base;
  b2ShapeDef def;
} box2d_b2ShapeDef_obj_t;

typedef struct box2d_b2Shape_obj_t
{
  mp_obj_base_t base;
  b2ShapeId shape;
} box2d_b2Shape_obj_t;

struct b2Polygon;

typedef struct {
  mp_obj_base_t base;
  b2Polygon polygon;
} box2d_b2Polygon_obj_t;

typedef struct box2d_bBodyDef_obj_t
{
  mp_obj_base_t base;
  b2BodyDef def;
} box2d_b2BodyDef_obj_t;

typedef struct box2d_b2Body_obj_t
{
  mp_obj_base_t base;
  b2BodyId body;
} box2d_b2Body_obj_t;


#include "wrapped_types.h"
#include "wrapped_world.h"
#include "wrapped_constraints.h"
#include "wrapped_shape.h"
#include "wrapped_body.h"

STATIC mp_obj_t test_init(mp_uint_t n_args, const mp_obj_t* args) {
    worldDef = b2DefaultWorldDef();
    test = b2CreateWorld(&worldDef);
    return mp_const_none;
}
MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(test_init_obj, 0, 0, test_init);

STATIC const mp_rom_map_elem_t mp_module_box2d_globals_table[] = {
    {MP_ROM_QSTR(MP_QSTR___name__), MP_ROM_QSTR(MP_QSTR_box2d)},
    {MP_ROM_QSTR(MP_QSTR_test_init), MP_ROM_PTR(&test_init_obj)},
    {MP_ROM_QSTR(MP_QSTR_b2Vec2), (mp_obj_t)&box2d_b2Vec2_type},
    {MP_ROM_QSTR(MP_QSTR_b2Rot), (mp_obj_t)&box2d_b2Rot_type},
    {MP_ROM_QSTR(MP_QSTR_b2Transform), (mp_obj_t)&box2d_b2Transform_type},
    {MP_ROM_QSTR(MP_QSTR_b2Mat22), (mp_obj_t)&box2d_b2Mat22_type},
    {MP_ROM_QSTR(MP_QSTR_b2AABB), (mp_obj_t)&box2d_b2AABB_type},
    {MP_ROM_QSTR(MP_QSTR_b2Polygon), (mp_obj_t)&box2d_b2Polygon_type},
    {MP_ROM_QSTR(MP_QSTR_b2RayCastInput), (mp_obj_t)&box2d_b2RayCastInput_type},
    {MP_ROM_QSTR(MP_QSTR_b2RayCastOutput), (mp_obj_t)&box2d_b2RayCastOutput_type},
    {MP_ROM_QSTR(MP_QSTR_b2WorldDef), (mp_obj_t)&box2d_b2WorldDef_type},
    {MP_ROM_QSTR(MP_QSTR_b2BodyDef), (mp_obj_t)&box2d_b2BodyDef_type},
    {MP_ROM_QSTR(MP_QSTR_b2ShapeDef), (mp_obj_t)&box2d_b2ShapeDef_type},
    {MP_ROM_QSTR(MP_QSTR_b2World), (mp_obj_t)&box2d_b2World_type},
    {MP_ROM_QSTR(MP_QSTR_b2Body), (mp_obj_t)&box2d_b2Body_type},
    {MP_ROM_QSTR(MP_QSTR_b2Joint), (mp_obj_t)&box2d_b2Joint_type},
    {MP_ROM_QSTR(MP_QSTR_b2Filter), (mp_obj_t)&box2d_b2Filter_type},
};

STATIC MP_DEFINE_CONST_DICT(mp_module_box2d_globals, mp_module_box2d_globals_table);

const mp_obj_module_t mp_module_box2d = {
    .base = { &mp_type_module },
    .globals = (mp_obj_dict_t *)&mp_module_box2d_globals,
};

MP_REGISTER_MODULE(MP_QSTR_box2d, mp_module_box2d);
