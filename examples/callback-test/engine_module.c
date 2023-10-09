#include <engine_module.h>

STATIC MP_DEFINE_CONST_FUN_OBJ_0(engine_start_obj, engine_start);

STATIC MP_DEFINE_CONST_FUN_OBJ_1(engine_set_tick_rate_obj, engine_set_tick_rate);
STATIC MP_DEFINE_CONST_FUN_OBJ_0(engine_get_tick_rate_obj, engine_get_tick_rate);

STATIC MP_DEFINE_CONST_FUN_OBJ_1(engine_set_physics_tick_rate_obj, engine_set_physics_tick_rate);
STATIC MP_DEFINE_CONST_FUN_OBJ_0(engine_get_physics_tick_rate_obj, engine_get_physics_tick_rate);


STATIC const mp_rom_map_elem_t engine_module_globals_table[] = {
    { MP_ROM_QSTR(MP_QSTR___name__), MP_ROM_QSTR(MP_QSTR_engine) },
    { MP_ROM_QSTR(MP_QSTR_start), MP_ROM_PTR(&engine_start_obj) },

    { MP_ROM_QSTR(MP_QSTR_set_tick_rate), MP_ROM_PTR(&engine_set_tick_rate_obj) },
    { MP_ROM_QSTR(MP_QSTR_get_tick_rate), MP_ROM_PTR(&engine_get_tick_rate_obj) },

    { MP_ROM_QSTR(MP_QSTR_set_physics_tick_rate), MP_ROM_PTR(&engine_set_physics_tick_rate_obj) },
    { MP_ROM_QSTR(MP_QSTR_get_physics_tick_rate), MP_ROM_PTR(&engine_get_physics_tick_rate_obj) },
};
STATIC MP_DEFINE_CONST_DICT(engine_module_globals, engine_module_globals_table);


// Define module object.
const mp_obj_module_t engine_user_cmodule = {
    .base = { &mp_type_module },
    .globals = (mp_obj_dict_t*)&engine_module_globals,
};

// Register the module to make it available in Python.
MP_REGISTER_MODULE(MP_QSTR_engine, engine_user_cmodule);
