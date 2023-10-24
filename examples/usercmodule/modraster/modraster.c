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
#include "hardware/interp.h"
#include <stdint.h>

STATIC mp_obj_t fill_buffer(mp_uint_t n_args, const mp_obj_t* args) {
    mp_obj_t buf = args[0];
    mp_buffer_info_t buf_inf;
    mp_get_buffer_raise(buf, &buf_inf, MP_BUFFER_WRITE);
    int bx = mp_obj_get_int(args[1]);
    int by = mp_obj_get_int(args[2]);
    uint16_t fill = 0;
    if(n_args == 4) fill = mp_obj_get_int(args[3]);
    fill = (fill >> 8) | (fill << 8);
    uint16_t* p = (uint16_t*)buf_inf.buf;
    for(int i = 0; i < bx*by; i++) *((uint16_t*)p++) = fill;
    return mp_const_none;
}
MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(fill_buffer_obj, 3, 4, fill_buffer);

int32_t inv32(const int32_t d) {return (d > 0) ?  (((uint32_t)(0xFFFFFFFF - d + 1) / d)) : -(((uint32_t)(0xFFFFFFFF + d + 1) / (-d)));}
void swap(int* a, int* b) {*a ^= *b; *b ^= *a; *a ^= *b;}

void init_interp(int t_xs_log2)
{
  interp_config cfg = interp_default_config();
  interp_config_set_signed(&cfg, false);
  interp_config_set_add_raw(&cfg, true);
  interp_config_set_shift(&cfg, 16-t_xs_log2);
  interp_config_set_mask(&cfg, t_xs_log2, 31);
  interp_set_config(interp0, 0, &cfg);
  interp_config_set_shift(&cfg, 16);
  interp_config_set_mask(&cfg, 0, 31);
  interp_set_config(interp0, 1, &cfg);
}

void scanlineuv(uint16_t* fb, int f_xs, uint16_t* tex, int t_xs, int x1, int x2, int y, int u1, int v1, int u2, int v2)
{
    if(x1 == x2) return;
    if(x1 > x2)
    {
        swap(&x1, &x2);
        swap(&u1, &u2);
        swap(&v1, &v2);
    }
    uint32_t cu = u1;
    uint32_t cv = v1;
    int32_t idx = inv32((int32_t)(x2) - x1);
    int32_t du = ((int64_t)(u2 - u1) * idx) >> 32;
    int32_t dv = ((int64_t)(v2 - v1) * idx) >> 32;
    size_t pos = (size_t)(y) * f_xs + x1;
    for(uint16_t x = x1; x < x2; x++)
    {
        fb[pos++] = tex[((cv += dv) >> 16) * t_xs + ((cu += du) >> 16)];
    }
}

void scanlineuv_pow2_tex(uint16_t* fb, int f_xs, uint16_t* tex, int t_xs_log2, int x1, int x2, int y, int u1, int v1, int u2, int v2)
{
    if(x1 == x2) return;
    if(x1 > x2)
    {
        swap(&x1, &x2);
        swap(&u1, &u2);
        swap(&v1, &v2);
    }
    uint32_t cu = u1;
    uint32_t cv = v1;
    int32_t idx = inv32((int32_t)(x2) - x1);
    int32_t du = ((int64_t)(u2 - u1) * idx) >> 32;
    int32_t dv = ((int64_t)(v2 - v1) * idx) >> 32;
    size_t pos = (size_t)(y) * f_xs + x1;
    // interp_config cfg = interp_default_config();
    // interp_config_set_signed(&cfg, false);
    // interp_config_set_add_raw(&cfg, true);
    // interp_config_set_shift(&cfg, 16-t_xs_log2);
    // interp_config_set_mask(&cfg, t_xs_log2, 31);
    interp0->accum[0] = cv+dv;
    interp0->base[0] = dv;
    //interp_set_config(interp0, 0, &cfg);
    interp0->accum[1] = cu+du;
    interp0->base[1] = du;
    // interp_config_set_shift(&cfg, 16);
    // interp_config_set_mask(&cfg, 0, 31);
    // interp_set_config(interp0, 1, &cfg);
    //int s1 = 16-t_xs_log2;
    //int sm = ~((1u << t_xs_log2)-1);
    for(uint16_t x = x1; x < x2; x++) {
        fb[pos++] = tex[interp_pop_full_result(interp0)];
    }
}

STATIC mp_obj_t scanline_uv(mp_uint_t n_args, const mp_obj_t* args)
{
    mp_obj_t fb = args[0];
    mp_buffer_info_t buf_inf;
    mp_get_buffer_raise(fb, &buf_inf, MP_BUFFER_WRITE);
    int f_xs = mp_obj_get_int(args[1]);
    //int f_ys = mp_obj_get_int(args[2]);
    mp_obj_t tex = args[3];
    mp_buffer_info_t tex_inf;
    mp_get_buffer_raise(tex, &tex_inf, MP_BUFFER_WRITE);
    int t_xs = mp_obj_get_int(args[4]);
    int x1 = mp_obj_get_int(args[6]);
    int x2 = mp_obj_get_int(args[7]);
    int y = mp_obj_get_int(args[8]);
    int u1 = mp_obj_get_int(args[9]);
    int v1 = mp_obj_get_int(args[10]);
    int u2 = mp_obj_get_int(args[11]);
    int v2 = mp_obj_get_int(args[12]);
    scanlineuv(buf_inf.buf, f_xs, tex_inf.buf, t_xs, x1, x2, y, u1, v1, u2, v2);
    return mp_const_none;
}
MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(scanline_uv_obj, 13, 13, scanline_uv);

void drawtriangle(uint16_t* fb, int f_xs, uint16_t* tex, int t_xs, int x1, int y1, int x2, int y2, int x3, int y3, int u1, int v1, int u2, int v2, int u3, int v3)
{
    if(y1 > y2)
    {
        swap(&x1, &x2);
        swap(&y1, &y2);
        swap(&u1, &u2);
        swap(&v1, &v2);
    }
    if(y1 > y3)
    {
        swap(&x1, &x3);
        swap(&y1, &y3);
        swap(&u1, &u3);
        swap(&v1, &v3);
    }
    if(y2 > y3)
    {
        swap(&x2, &x3);
        swap(&y2, &y3);
        swap(&u2, &u3);
        swap(&v2, &v3);
    }
    int32_t cx1 = 0;
    int32_t cx2 = 0;
    int32_t cu1 = 0;
    int32_t cu2 = 0;
    int32_t cv1 = 0;
    int32_t cv2 = 0;
    int32_t dx1 = 0;
    int32_t dx2 = 0;
    int32_t du1 = 0;
    int32_t du2 = 0;
    int32_t dv1 = 0;
    int32_t dv2 = 0;
    if(y2 != y1)
    {
        int32_t idy21 = inv32(y2 - y1);
        dx1 = (((int64_t)(x2) - x1) * idy21) >> 16;
        du1 = (((int64_t)(u2) - u1) * idy21) >> 16;
        dv1 = (((int64_t)(v2) - v1) * idy21) >> 16;
    }
    if(y3 != y1)
    {
        int32_t idy31 = inv32(y3 - y1);
        dx2 = (((int64_t)(x3) - x1) * idy31) >> 16;
        du2 = (((int64_t)(u3) - u1) * idy31) >> 16;
        dv2 = (((int64_t)(v3) - v1) * idy31) >> 16;
    }
    cx1 = cx2 = (int32_t)(x1) << 16;
    cu1 = cu2 = (int32_t)(u1) << 16;
    cv1 = cv2 = (int32_t)(v1) << 16;
    for(uint16_t y = y1; y < y2; y++)
    {
        scanlineuv(fb, f_xs, tex, t_xs, cx1 >> 16, cx2 >> 16, y, cu1, cv1, cu2, cv2);
        cx1 += dx1;
        cx2 += dx2;
        cu1 += du1;
        cv1 += dv1;
        cu2 += du2;
        cv2 += dv2;
    }
    cx1 = (int32_t)(x2) << 16;
    cu1 = (int32_t)(u2) << 16;
    cv1 = (int32_t)(v2) << 16;
    if(y3 != y2)
    {
        int32_t idy32 = inv32(y3 - y2);
        dx1 = (((int64_t)(x3) - x2) * idy32) >> 16;
        du1 = (((int64_t)(u3) - u2) * idy32) >> 16;
        dv1 = (((int64_t)(v3) - v2) * idy32) >> 16;
    }
    for(uint16_t y = y2; y < y3; y++)
    {
        scanlineuv(fb, f_xs, tex, t_xs, cx1 >> 16, cx2 >> 16, y, cu1, cv1, cu2, cv2);
        cx1 += dx1;
        cx2 += dx2;
        cu1 += du1;
        cv1 += dv1;
        cu2 += du2;
        cv2 += dv2;
    }
}

void drawtriangle_pow2_tex(uint16_t* fb, int f_xs, uint16_t* tex, int t_xs_log2, int x1, int y1, int x2, int y2, int x3, int y3, int u1, int v1, int u2, int v2, int u3, int v3)
{
    if(y1 > y2)
    {
        swap(&x1, &x2);
        swap(&y1, &y2);
        swap(&u1, &u2);
        swap(&v1, &v2);
    }
    if(y1 > y3)
    {
        swap(&x1, &x3);
        swap(&y1, &y3);
        swap(&u1, &u3);
        swap(&v1, &v3);
    }
    if(y2 > y3)
    {
        swap(&x2, &x3);
        swap(&y2, &y3);
        swap(&u2, &u3);
        swap(&v2, &v3);
    }
    int32_t cx1 = 0;
    int32_t cx2 = 0;
    int32_t cu1 = 0;
    int32_t cu2 = 0;
    int32_t cv1 = 0;
    int32_t cv2 = 0;
    int32_t dx1 = 0;
    int32_t dx2 = 0;
    int32_t du1 = 0;
    int32_t du2 = 0;
    int32_t dv1 = 0;
    int32_t dv2 = 0;
    if(y2 != y1)
    {
        int32_t idy21 = inv32(y2 - y1);
        dx1 = (((int64_t)(x2) - x1) * idy21) >> 16;
        du1 = (((int64_t)(u2) - u1) * idy21) >> 16;
        dv1 = (((int64_t)(v2) - v1) * idy21) >> 16;
    }
    if(y3 != y1)
    {
        int32_t idy31 = inv32(y3 - y1);
        dx2 = (((int64_t)(x3) - x1) * idy31) >> 16;
        du2 = (((int64_t)(u3) - u1) * idy31) >> 16;
        dv2 = (((int64_t)(v3) - v1) * idy31) >> 16;
    }
    cx1 = cx2 = (int32_t)(x1) << 16;
    cu1 = cu2 = (int32_t)(u1) << 16;
    cv1 = cv2 = (int32_t)(v1) << 16;
    for(uint16_t y = y1; y < y2; y++)
    {
        scanlineuv_pow2_tex(fb, f_xs, tex, t_xs_log2, cx1 >> 16, cx2 >> 16, y, cu1, cv1, cu2, cv2);
        cx1 += dx1;
        cx2 += dx2;
        cu1 += du1;
        cv1 += dv1;
        cu2 += du2;
        cv2 += dv2;
    }
    cx1 = (int32_t)(x2) << 16;
    cu1 = (int32_t)(u2) << 16;
    cv1 = (int32_t)(v2) << 16;
    if(y3 != y2)
    {
        int32_t idy32 = inv32(y3 - y2);
        dx1 = (((int64_t)(x3) - x2) * idy32) >> 16;
        du1 = (((int64_t)(u3) - u2) * idy32) >> 16;
        dv1 = (((int64_t)(v3) - v2) * idy32) >> 16;
    }
    for(uint16_t y = y2; y < y3; y++)
    {
        scanlineuv_pow2_tex(fb, f_xs, tex, t_xs_log2, cx1 >> 16, cx2 >> 16, y, cu1, cv1, cu2, cv2);
        cx1 += dx1;
        cx2 += dx2;
        cu1 += du1;
        cv1 += dv1;
        cu2 += du2;
        cv2 += dv2;
    }
}

STATIC mp_obj_t draw_triangle(mp_uint_t n_args, const mp_obj_t* args)
{
    mp_obj_t fb = args[0];
    mp_buffer_info_t buf_inf;
    mp_get_buffer_raise(fb, &buf_inf, MP_BUFFER_WRITE);
    int f_xs = mp_obj_get_int(args[1]);
    //int f_ys = mp_obj_get_int(args[2]);
    mp_obj_t tex = args[3];
    mp_buffer_info_t tex_inf;
    mp_get_buffer_raise(tex, &tex_inf, MP_BUFFER_READ);
    int t_xs = mp_obj_get_int(args[4]);
    //int t_ys = mp_obj_get_int(args[5]);
    const int x1 = MP_OBJ_SMALL_INT_VALUE(args[6]);
    const int y1 = MP_OBJ_SMALL_INT_VALUE(args[7]);
    const int x2 = MP_OBJ_SMALL_INT_VALUE(args[8]);
    const int y2 = MP_OBJ_SMALL_INT_VALUE(args[9]);
    const int x3 = MP_OBJ_SMALL_INT_VALUE(args[10]);
    const int y3 = MP_OBJ_SMALL_INT_VALUE(args[11]);
    const int u1 = MP_OBJ_SMALL_INT_VALUE(args[12]);
    const int v1 = MP_OBJ_SMALL_INT_VALUE(args[13]);
    const int u2 = MP_OBJ_SMALL_INT_VALUE(args[14]);
    const int v2 = MP_OBJ_SMALL_INT_VALUE(args[15]);
    const int u3 = MP_OBJ_SMALL_INT_VALUE(args[16]);
    const int v3 = MP_OBJ_SMALL_INT_VALUE(args[17]);
    drawtriangle(buf_inf.buf, f_xs, tex_inf.buf, t_xs, x1, y1, x2, y2, x3, y3, u1, v1, u2, v2, u3, v3);
    return mp_const_none;
}
MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(draw_triangle_obj, 18, 18, draw_triangle);

STATIC mp_obj_t draw_triangle_pow2_tex(mp_uint_t n_args, const mp_obj_t* args)
{
    mp_obj_t fb = args[0];
    mp_buffer_info_t buf_inf;
    mp_get_buffer_raise(fb, &buf_inf, MP_BUFFER_WRITE);
    int f_xs = mp_obj_get_int(args[1]);
    //int f_ys = mp_obj_get_int(args[2]);
    mp_obj_t tex = args[3];
    mp_buffer_info_t tex_inf;
    mp_get_buffer_raise(tex, &tex_inf, MP_BUFFER_READ);
    int t_xs = mp_obj_get_int(args[4]);
    //int t_ys = mp_obj_get_int(args[5]);
    const int x1 = MP_OBJ_SMALL_INT_VALUE(args[6]);
    const int y1 = MP_OBJ_SMALL_INT_VALUE(args[7]);
    const int x2 = MP_OBJ_SMALL_INT_VALUE(args[8]);
    const int y2 = MP_OBJ_SMALL_INT_VALUE(args[9]);
    const int x3 = MP_OBJ_SMALL_INT_VALUE(args[10]);
    const int y3 = MP_OBJ_SMALL_INT_VALUE(args[11]);
    const int u1 = MP_OBJ_SMALL_INT_VALUE(args[12]);
    const int v1 = MP_OBJ_SMALL_INT_VALUE(args[13]);
    const int u2 = MP_OBJ_SMALL_INT_VALUE(args[14]);
    const int v2 = MP_OBJ_SMALL_INT_VALUE(args[15]);
    const int u3 = MP_OBJ_SMALL_INT_VALUE(args[16]);
    const int v3 = MP_OBJ_SMALL_INT_VALUE(args[17]);
    init_interp(t_xs);
    drawtriangle_pow2_tex(buf_inf.buf, f_xs, tex_inf.buf, t_xs, x1, y1, x2, y2, x3, y3, u1, v1, u2, v2, u3, v3);
    return mp_const_none;
}
MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(draw_triangle_pow2_tex_obj, 18, 18, draw_triangle_pow2_tex);

STATIC mp_obj_t draw_triangles_pow2_tex(mp_uint_t n_args, const mp_obj_t* args)
{
    mp_obj_t fb = args[0];
    mp_buffer_info_t buf_inf;
    mp_get_buffer_raise(fb, &buf_inf, MP_BUFFER_WRITE);
    int f_xs = mp_obj_get_int(args[1]);
    //int f_ys = mp_obj_get_int(args[2]);
    mp_obj_t tex = args[3];
    mp_buffer_info_t tex_inf;
    mp_get_buffer_raise(tex, &tex_inf, MP_BUFFER_READ);
    int t_xs = mp_obj_get_int(args[4]);
    //int t_ys = mp_obj_get_int(args[5]);
    mp_obj_t* v_objs;
    size_t v_obj_len;
    size_t v_pos = 0;
    mp_obj_get_array(args[6], &v_obj_len, &v_objs);
    mp_obj_t* uv_objs;
    size_t uv_obj_len;
    size_t uv_pos = 0;
    mp_obj_get_array(args[7], &uv_obj_len, &uv_objs);
    init_interp(t_xs);
    while((v_obj_len - v_pos) >= 6 && (uv_obj_len - uv_pos) >= 6)
    {
      const int x1 = MP_OBJ_SMALL_INT_VALUE(v_objs[v_pos++]);
      const int y1 = MP_OBJ_SMALL_INT_VALUE(v_objs[v_pos++]);
      const int x2 = MP_OBJ_SMALL_INT_VALUE(v_objs[v_pos++]);
      const int y2 = MP_OBJ_SMALL_INT_VALUE(v_objs[v_pos++]);
      const int x3 = MP_OBJ_SMALL_INT_VALUE(v_objs[v_pos++]);
      const int y3 = MP_OBJ_SMALL_INT_VALUE(v_objs[v_pos++]);
      const int u1 = MP_OBJ_SMALL_INT_VALUE(uv_objs[uv_pos++]);
      const int v1 = MP_OBJ_SMALL_INT_VALUE(uv_objs[uv_pos++]);
      const int u2 = MP_OBJ_SMALL_INT_VALUE(uv_objs[uv_pos++]);
      const int v2 = MP_OBJ_SMALL_INT_VALUE(uv_objs[uv_pos++]);
      const int u3 = MP_OBJ_SMALL_INT_VALUE(uv_objs[uv_pos++]);
      const int v3 = MP_OBJ_SMALL_INT_VALUE(uv_objs[uv_pos++]);
      drawtriangle_pow2_tex(buf_inf.buf, f_xs, tex_inf.buf, t_xs, x1, y1, x2, y2, x3, y3, u1, v1, u2, v2, u3, v3);
    }
    return mp_const_none;
}
MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(draw_triangles_pow2_tex_obj, 8, 8, draw_triangles_pow2_tex);

STATIC mp_obj_t draw_triangles(mp_uint_t n_args, const mp_obj_t* args)
{
    mp_obj_t fb = args[0];
    mp_buffer_info_t buf_inf;
    mp_get_buffer_raise(fb, &buf_inf, MP_BUFFER_WRITE);
    int f_xs = mp_obj_get_int(args[1]);
    //int f_ys = mp_obj_get_int(args[2]);
    mp_obj_t tex = args[3];
    mp_buffer_info_t tex_inf;
    mp_get_buffer_raise(tex, &tex_inf, MP_BUFFER_READ);
    int t_xs = mp_obj_get_int(args[4]);
    //int t_ys = mp_obj_get_int(args[5]);
    mp_obj_t* v_objs;
    size_t v_obj_len;
    size_t v_pos = 0;
    mp_obj_get_array(args[6], &v_obj_len, &v_objs);
    mp_obj_t* uv_objs;
    size_t uv_obj_len;
    size_t uv_pos = 0;
    mp_obj_get_array(args[7], &uv_obj_len, &uv_objs);
    while((v_obj_len - v_pos) >= 6 && (uv_obj_len - uv_pos) >= 6)
    {
      const int x1 = MP_OBJ_SMALL_INT_VALUE(v_objs[v_pos++]);
      const int y1 = MP_OBJ_SMALL_INT_VALUE(v_objs[v_pos++]);
      const int x2 = MP_OBJ_SMALL_INT_VALUE(v_objs[v_pos++]);
      const int y2 = MP_OBJ_SMALL_INT_VALUE(v_objs[v_pos++]);
      const int x3 = MP_OBJ_SMALL_INT_VALUE(v_objs[v_pos++]);
      const int y3 = MP_OBJ_SMALL_INT_VALUE(v_objs[v_pos++]);
      const int u1 = MP_OBJ_SMALL_INT_VALUE(uv_objs[uv_pos++]);
      const int v1 = MP_OBJ_SMALL_INT_VALUE(uv_objs[uv_pos++]);
      const int u2 = MP_OBJ_SMALL_INT_VALUE(uv_objs[uv_pos++]);
      const int v2 = MP_OBJ_SMALL_INT_VALUE(uv_objs[uv_pos++]);
      const int u3 = MP_OBJ_SMALL_INT_VALUE(uv_objs[uv_pos++]);
      const int v3 = MP_OBJ_SMALL_INT_VALUE(uv_objs[uv_pos++]);
      drawtriangle(buf_inf.buf, f_xs, tex_inf.buf, t_xs, x1, y1, x2, y2, x3, y3, u1, v1, u2, v2, u3, v3);
    }
    return mp_const_none;
}
MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(draw_triangles_obj, 8, 8, draw_triangles);

STATIC const mp_rom_map_elem_t mp_module_raster_globals_table[] = {
    { MP_ROM_QSTR(MP_QSTR___name__), MP_ROM_QSTR(MP_QSTR_raster) },
    { MP_ROM_QSTR(MP_QSTR_fill_buffer), MP_ROM_PTR(&fill_buffer_obj) },
    { MP_ROM_QSTR(MP_QSTR_scanline_uv), MP_ROM_PTR(&scanline_uv_obj) },
    { MP_ROM_QSTR(MP_QSTR_draw_triangle), MP_ROM_PTR(&draw_triangle_obj) },
    { MP_ROM_QSTR(MP_QSTR_draw_triangle_pow2_tex), MP_ROM_PTR(&draw_triangle_pow2_tex_obj) },
    { MP_ROM_QSTR(MP_QSTR_draw_triangles), MP_ROM_PTR(&draw_triangles_obj) },
    { MP_ROM_QSTR(MP_QSTR_draw_triangles_pow2_tex), MP_ROM_PTR(&draw_triangles_pow2_tex_obj) },
};

STATIC MP_DEFINE_CONST_DICT(mp_module_raster_globals, mp_module_raster_globals_table);

const mp_obj_module_t mp_module_raster = {
    .base = { &mp_type_module },
    .globals = (mp_obj_dict_t *)&mp_module_raster_globals,
};

MP_REGISTER_MODULE(MP_QSTR_raster, mp_module_raster);
