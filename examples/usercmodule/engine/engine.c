#include <stdio.h>

#include "engine_object_layers.h"
#include "display/engine_display.h"
#include "engine_cameras.h"

#ifdef __unix__
    #include "display/engine_display_driver_unix_sdl.h"
#else
    #include "display/engine_display_driver_rp2_st7789.h"
#endif

#include "math/vector2.h"
#include "math/vector3.h"
#include "math/rectangle.h"

#include "nodes/empty_node.h"
#include "nodes/camera_node.h"
#include "nodes/bitmap_sprite_node.h"
#include "nodes/2d/rectangle_2d_node.h"

// ### MODULE ###

// Module functions
STATIC mp_obj_t engine_start(){
    ENGINE_INFO_PRINTF("Engine init");
    engine_display_init();
    engine_display_send();
    // engine_display_send();

    ENGINE_INFO_PRINTF("Engine starting...");

    while(true){
        ENGINE_PERFORMANCE_STOP(ENGINE_PERF_TIMER_1, "Loop time");
        ENGINE_PERFORMANCE_START(ENGINE_PERF_TIMER_1);
        engine_invoke_all_node_callbacks();

        // After every game cycle send the current active screen buffer to the display
        engine_display_send();
    }

    return mp_const_none;
}
MP_DEFINE_CONST_FUN_OBJ_0(engine_start_obj, engine_start);

static mp_obj_t engine_display_init_internal() {
    engine_display_init();
}
MP_DEFINE_CONST_FUN_OBJ_0(engine_display_init_obj, engine_display_init_internal);

static mp_obj_t engine_display_dma_wait_internal() {
    engine_display_st7789_dmawait();
}
MP_DEFINE_CONST_FUN_OBJ_0(engine_display_dma_wait_obj, engine_display_dma_wait_internal);

static mp_obj_t engine_display_push_fb(mp_obj_t fb) {
    mp_buffer_info_t buf_inf;
    mp_get_buffer_raise(fb, &buf_inf, MP_BUFFER_WRITE);
    engine_display_st7789_update((uint16_t*)buf_inf.buf);
}
MP_DEFINE_CONST_FUN_OBJ_1(engine_display_push_fb_obj, engine_display_push_fb);

STATIC mp_obj_t engine_debug_disable_all(){
    DEBUG_INFO_ENABLED = false;
    DEBUG_WARNINGS_ENABLED = false;
    DEBUG_ERRORS_ENABLED = false;
    DEBUG_PERFORMANCE_ENABLED = false;
    ENGINE_FORCE_PRINTF("Disabled all debug prints");
    return mp_const_none;
}
MP_DEFINE_CONST_FUN_OBJ_0(engine_debug_disable_all_obj, engine_debug_disable_all);


STATIC mp_obj_t engine_debug_enable_all(){
    DEBUG_INFO_ENABLED = true;
    DEBUG_WARNINGS_ENABLED = true;
    DEBUG_ERRORS_ENABLED = true;
    DEBUG_PERFORMANCE_ENABLED = true;
    ENGINE_FORCE_PRINTF("Enabled all debug prints");
    return mp_const_none;
}
MP_DEFINE_CONST_FUN_OBJ_0(engine_debug_enable_all_obj, engine_debug_enable_all);


STATIC mp_obj_t engine_enable_debug_setting(mp_obj_t debug_setting){
    // Translate the debug level and check if inbounds
    uint8_t engine_debug_setting = mp_obj_get_int(debug_setting);

    switch(engine_debug_setting){
        case DEBUG_SETTING_INFO:
            DEBUG_INFO_ENABLED = true;
            ENGINE_FORCE_PRINTF("Enabled info debug prints");
        break;
        case DEBUG_SETTING_WARNINGS:
            DEBUG_WARNINGS_ENABLED = true;
            ENGINE_FORCE_PRINTF("Enabled warning debug prints");
        break;
        case DEBUG_SETTING_ERRORS:
            DEBUG_ERRORS_ENABLED = true;
            ENGINE_FORCE_PRINTF("Enabled error debug prints");
        break;
        case DEBUG_SETTING_PERFORMANCE:
            DEBUG_PERFORMANCE_ENABLED = true;
            ENGINE_FORCE_PRINTF("Enabled performance debug prints");
        break;
    }

    return mp_const_none;
}
MP_DEFINE_CONST_FUN_OBJ_1(engine_enable_debug_setting_obj, engine_enable_debug_setting);


STATIC mp_obj_t engine_init(){

    return mp_const_none;
}
MP_DEFINE_CONST_FUN_OBJ_0(engine_init_obj, engine_init);


// Module attributes
STATIC const mp_rom_map_elem_t engine_globals_table[] = {
    { MP_OBJ_NEW_QSTR(MP_QSTR___name__), MP_OBJ_NEW_QSTR(MP_QSTR_engine) },
    { MP_OBJ_NEW_QSTR(MP_QSTR___init__), (mp_obj_t)&engine_init_obj },
    { MP_OBJ_NEW_QSTR(MP_QSTR_Vector2), (mp_obj_t)&vector2_class_type },
    { MP_OBJ_NEW_QSTR(MP_QSTR_Vector3), (mp_obj_t)&vector3_class_type },
    { MP_OBJ_NEW_QSTR(MP_QSTR_Rectangle), (mp_obj_t)&rectangle_class_type },
    { MP_OBJ_NEW_QSTR(MP_QSTR_EmptyNode), (mp_obj_t)&engine_empty_node_class_type },
    { MP_OBJ_NEW_QSTR(MP_QSTR_CameraNode), (mp_obj_t)&engine_camera_node_class_type },
    { MP_OBJ_NEW_QSTR(MP_QSTR_BitmapSpriteNode), (mp_obj_t)&engine_bitmap_sprite_node_class_type },
    { MP_OBJ_NEW_QSTR(MP_QSTR_Rectangle2DNode), (mp_obj_t)&engine_rectangle_2d_node_class_type },
    { MP_OBJ_NEW_QSTR(MP_QSTR_start), (mp_obj_t)&engine_start_obj },
    { MP_OBJ_NEW_QSTR(MP_QSTR_InitDisplay), (mp_obj_t)&engine_display_init_obj },
    { MP_OBJ_NEW_QSTR(MP_QSTR_DisplayDMAWait), (mp_obj_t)&engine_display_dma_wait_obj },
    { MP_OBJ_NEW_QSTR(MP_QSTR_PushFrame), (mp_obj_t)&engine_display_push_fb_obj },

    { MP_OBJ_NEW_QSTR(MP_QSTR_debug_enable_all), (mp_obj_t)&engine_debug_enable_all_obj },
    { MP_OBJ_NEW_QSTR(MP_QSTR_debug_disable_all), (mp_obj_t)&engine_debug_disable_all_obj },
    { MP_OBJ_NEW_QSTR(MP_QSTR_debug_enable_setting), (mp_obj_t)&engine_enable_debug_setting_obj },
    { MP_ROM_QSTR(MP_QSTR_debug_setting_info), MP_ROM_INT(DEBUG_SETTING_INFO) },
    { MP_ROM_QSTR(MP_QSTR_debug_setting_warnings), MP_ROM_INT(DEBUG_SETTING_WARNINGS) },
    { MP_ROM_QSTR(MP_QSTR_debug_setting_errors), MP_ROM_INT(DEBUG_SETTING_ERRORS) },
    { MP_ROM_QSTR(MP_QSTR_debug_setting_performance), MP_ROM_INT(DEBUG_SETTING_PERFORMANCE) },
};

// Module init
STATIC MP_DEFINE_CONST_DICT (mp_module_engine_globals, engine_globals_table);

const mp_obj_module_t engine_user_cmodule = {
    .base = { &mp_type_module },
    .globals = (mp_obj_dict_t*)&mp_module_engine_globals,
};

MP_REGISTER_MODULE(MP_QSTR_engine, engine_user_cmodule);
