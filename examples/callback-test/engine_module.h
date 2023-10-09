#ifndef ENGINE_MODULE_H
#define ENGINE_MODULE_H

#include "py/obj.h"
#include "py/runtime.h"
#include "py/builtin.h"

extern mp_obj_t engine_start();

extern mp_obj_t engine_set_tick_rate(mp_obj_t new_tick_rate);
extern mp_obj_t engine_get_tick_rate();

extern mp_obj_t engine_set_physics_tick_rate(mp_obj_t new_physics_tick_rate);
extern mp_obj_t engine_get_physics_tick_rate();

#endif  // ENGINE_MODULE_H