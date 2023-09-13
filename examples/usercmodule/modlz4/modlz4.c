/*
 * This file is part of the MicroPython project, http://micropython.org/
 *
 * The MIT License (MIT)
 *
 * Copyright (c) 2013, 2014 Damien P. George
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include "py/builtin.h"
#include "py/runtime.h"
#include "py/mpstate.h"
#include "py/obj.h"

#include "lz4.h"

STATIC mp_obj_t compress_block(mp_uint_t n_args, const mp_obj_t* args) {
    mp_obj_t in_buf = args[0];
    int accel = 1;
    if(n_args == 2)
    {
      accel = mp_obj_get_int(args[1]);
    }
    mp_buffer_info_t ib_inf;
    mp_get_buffer_raise(in_buf, &ib_inf, MP_BUFFER_READ);

    vstr_t vstr;
    vstr_init_len(&vstr, LZ4_COMPRESSBOUND(ib_inf.len));

    int size = LZ4_compress_fast((char*)ib_inf.buf, (char*)vstr.buf, ib_inf.len, LZ4_COMPRESSBOUND(ib_inf.len), accel);
    m_realloc(vstr.buf, size);
    vstr.len = size;
    return mp_obj_new_str_from_vstr(&mp_type_bytes, &vstr);
}
MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(compress_block_obj, 1, 2, compress_block);

STATIC mp_obj_t compress_block_into(mp_uint_t n_args, const mp_obj_t* args) {
    mp_obj_t in_buf = args[0];
    mp_buffer_info_t ib_inf;
    mp_get_buffer_raise(in_buf, &ib_inf, MP_BUFFER_READ);
    mp_obj_t out_buf = args[1];
    mp_buffer_info_t ob_inf;
    mp_get_buffer_raise(out_buf, &ob_inf, MP_BUFFER_WRITE);
    int accel = 1;
    if(n_args == 3)
    {
      accel = mp_obj_get_int(args[2]);
    }

    if(ob_inf.len < LZ4_COMPRESSBOUND(ib_inf.len))
    {
        ob_inf.len = LZ4_COMPRESSBOUND(ib_inf.len);
        m_realloc(ob_inf.buf, LZ4_COMPRESSBOUND(ib_inf.len));
    }

    int size = LZ4_compress_fast((char*)ib_inf.buf, (char*)ob_inf.buf, ib_inf.len, LZ4_COMPRESSBOUND(ib_inf.len), accel);
    m_realloc(ob_inf.buf, size);
    ob_inf.len = size;
    return mp_obj_new_int(size);
}
MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(compress_block_into_obj, 2, 3, compress_block_into);

STATIC mp_obj_t decompress_block(mp_uint_t n_args, const mp_obj_t* args) {
    mp_obj_t in_buf = args[0];
    int dc_size = mp_obj_get_int(args[1]);
    mp_buffer_info_t ib_inf;
    mp_get_buffer_raise(in_buf, &ib_inf, MP_BUFFER_READ);

    vstr_t vstr;
    vstr_init_len(&vstr, dc_size);

    //int size = LZ4_compress_default((char*)ib_inf.buf, (char*)vstr.buf, ib_inf.len, LZ4_COMPRESSBOUND(ib_inf.len));
    dc_size = LZ4_decompress_safe(ib_inf.buf, (char*)vstr.buf, ib_inf.len, dc_size);
    return mp_obj_new_str_from_vstr(&mp_type_bytes, &vstr);
}
MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(decompress_block_obj, 2, 2, decompress_block);

STATIC const mp_rom_map_elem_t mp_module_lz4_globals_table[] = {
    { MP_ROM_QSTR(MP_QSTR___name__), MP_ROM_QSTR(MP_QSTR_lz4) },
    { MP_ROM_QSTR(MP_QSTR_compress_block), MP_ROM_PTR(&compress_block_obj) },
    { MP_ROM_QSTR(MP_QSTR_decompress_block), MP_ROM_PTR(&decompress_block_obj) },
    { MP_ROM_QSTR(MP_QSTR_compress_block_into), MP_ROM_PTR(&compress_block_into_obj) },
};

STATIC MP_DEFINE_CONST_DICT(mp_module_lz4_globals, mp_module_lz4_globals_table);

const mp_obj_module_t mp_module_lz4 = {
    .base = { &mp_type_module },
    .globals = (mp_obj_dict_t *)&mp_module_lz4_globals,
};

MP_REGISTER_MODULE(MP_QSTR_lz4, mp_module_lz4);
