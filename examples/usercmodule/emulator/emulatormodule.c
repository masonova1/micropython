// Include MicroPython API.
#include "py/runtime.h"


void __noinline screen_breakpoint(uint32_t r0) {
    asm("bkpt 28");
}


void __noinline audio_breakpoint(uint32_t r0) {
    asm("bkpt 29");
}


void __noinline brightness_breakpoint(uint32_t r0) {
    asm("bkpt 30");
}


void __noinline update_breakpoint() {
    asm("bkpt 31");
}


STATIC mp_obj_t emulator_screen_breakpoint(mp_obj_t buffer_addr16_obj) {
    screen_breakpoint(mp_obj_get_int(buffer_addr16_obj));
    return 0;
}
STATIC MP_DEFINE_CONST_FUN_OBJ_1(emulator_screen_breakpoint_obj, emulator_screen_breakpoint);


STATIC mp_obj_t emulator_audio_breakpoint(mp_obj_t freq16_obj) {
    audio_breakpoint(mp_obj_get_int(freq16_obj));
    return 0;
}
STATIC MP_DEFINE_CONST_FUN_OBJ_1(emulator_audio_breakpoint_obj, emulator_audio_breakpoint);


STATIC mp_obj_t emulator_brightness_breakpoint(mp_obj_t brightness8_obj) {
    brightness_breakpoint(mp_obj_get_int(brightness8_obj));
    return 0;
}
STATIC MP_DEFINE_CONST_FUN_OBJ_1(emulator_brightness_breakpoint_obj, emulator_brightness_breakpoint);


STATIC mp_obj_t emulator_update_breakpoint() {
    update_breakpoint();
    return 0;
}
STATIC MP_DEFINE_CONST_FUN_OBJ_0(emulator_update_breakpoint_obj, emulator_update_breakpoint);


// Define all properties of the module.
// Table entries are key/value pairs of the attribute name (a string)
// and the MicroPython object reference.
// All identifiers and strings are written as MP_QSTR_xxx and will be
// optimized to word-sized integers by the build system (interned strings).
STATIC const mp_rom_map_elem_t emulator_module_globals_table[] = {
    { MP_ROM_QSTR(MP_QSTR___name__), MP_ROM_QSTR(MP_QSTR_emulator) },
    { MP_ROM_QSTR(MP_QSTR_screen_breakpoint), MP_ROM_PTR(&emulator_screen_breakpoint_obj) },
    { MP_ROM_QSTR(MP_QSTR_audio_breakpoint), MP_ROM_PTR(&emulator_audio_breakpoint_obj) },
    { MP_ROM_QSTR(MP_QSTR_brightness_breakpoint), MP_ROM_PTR(&emulator_brightness_breakpoint_obj) },
    { MP_ROM_QSTR(MP_QSTR_update_breakpoint), MP_ROM_PTR(&emulator_update_breakpoint_obj) },
};
STATIC MP_DEFINE_CONST_DICT(emulator_module_globals, emulator_module_globals_table);




// Define module object.
const mp_obj_module_t emulator_user_cmodule = {
    .base = { &mp_type_module },
    .globals = (mp_obj_dict_t *)&emulator_module_globals,
};

// Register the module to make it available in Python.
// Note: the "1" in the third argument means this module is always enabled.
// This "1" can be optionally replaced with a macro like MODULE_CEXAMPLE_ENABLED
// which can then be used to conditionally enable this module.
MP_REGISTER_MODULE(MP_QSTR_emulator, emulator_user_cmodule);
