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
    //fill = (fill >> 8) | (fill << 8);
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

const static uint16_t sin_tan_tab[512] = {
    0, 0,
    201, 402,
    402, 804,
    603, 1206,
    804, 1608,
    1005, 2010,
    1207, 2412,
    1408, 2814,
    1609, 3216,
    1810, 3617,
    2011, 4019,
    2213, 4420,
    2414, 4821,
    2615, 5222,
    2817, 5623,
    3018, 6023,
    3220, 6424,
    3421, 6824,
    3623, 7224,
    3825, 7623,
    4026, 8022,
    4228, 8421,
    4430, 8820,
    4632, 9218,
    4834, 9616,
    5036, 10014,
    5239, 10411,
    5441, 10808,
    5644, 11204,
    5846, 11600,
    6049, 11996,
    6252, 12391,
    6455, 12785,
    6658, 13180,
    6861, 13573,
    7064, 13966,
    7268, 14359,
    7471, 14751,
    7675, 15143,
    7879, 15534,
    8083, 15924,
    8287, 16314,
    8492, 16703,
    8696, 17091,
    8901, 17479,
    9106, 17867,
    9311, 18253,
    9516, 18639,
    9721, 19024,
    9927, 19409,
    10133, 19792,
    10339, 20175,
    10545, 20557,
    10751, 20939,
    10958, 21320,
    11165, 21699,
    11372, 22078,
    11579, 22457,
    11786, 22834,
    11994, 23210,
    12202, 23586,
    12410, 23961,
    12618, 24335,
    12827, 24708,
    13036, 25080,
    13245, 25451,
    13454, 25821,
    13664, 26190,
    13874, 26558,
    14084, 26925,
    14295, 27291,
    14506, 27656,
    14717, 28020,
    14928, 28383,
    15140, 28745,
    15352, 29106,
    15564, 29466,
    15776, 29824,
    15989, 30182,
    16202, 30538,
    16416, 30893,
    16630, 31248,
    16844, 31600,
    17058, 31952,
    17273, 32303,
    17489, 32652,
    17704, 33000,
    17920, 33347,
    18136, 33692,
    18353, 34037,
    18570, 34380,
    18787, 34721,
    19005, 35062,
    19223, 35401,
    19442, 35738,
    19661, 36075,
    19880, 36410,
    20100, 36744,
    20320, 37076,
    20541, 37407,
    20762, 37736,
    20983, 38064,
    21205, 38391,
    21427, 38716,
    21650, 39040,
    21873, 39362,
    22097, 39683,
    22321, 40002,
    22546, 40320,
    22771, 40636,
    22997, 40951,
    23223, 41264,
    23449, 41576,
    23676, 41886,
    23904, 42194,
    24132, 42501,
    24360, 42806,
    24590, 43110,
    24819, 43412,
    25049, 43713,
    25280, 44011,
    25511, 44308,
    25743, 44604,
    25975, 44898,
    26208, 45190,
    26442, 45480,
    26676, 45769,
    26911, 46056,
    27146, 46341,
    27382, 46624,
    27618, 46906,
    27855, 47186,
    28093, 47464,
    28331, 47741,
    28570, 48015,
    28810, 48288,
    29050, 48559,
    29291, 48828,
    29533, 49095,
    29775, 49361,
    30018, 49624,
    30261, 49886,
    30506, 50146,
    30751, 50404,
    30996, 50660,
    31243, 50914,
    31490, 51166,
    31738, 51417,
    31986, 51665,
    32236, 51911,
    32486, 52156,
    32736, 52398,
    32988, 52639,
    33240, 52878,
    33494, 53114,
    33748, 53349,
    34002, 53581,
    34258, 53812,
    34514, 54040,
    34772, 54267,
    35030, 54491,
    35289, 54714,
    35548, 54934,
    35809, 55152,
    36071, 55368,
    36333, 55582,
    36596, 55794,
    36861, 56004,
    37126, 56212,
    37392, 56418,
    37659, 56621,
    37927, 56823,
    38196, 57022,
    38465, 57219,
    38736, 57414,
    39008, 57607,
    39281, 57798,
    39555, 57986,
    39829, 58172,
    40105, 58356,
    40382, 58538,
    40660, 58718,
    40939, 58896,
    41219, 59071,
    41500, 59244,
    41782, 59415,
    42066, 59583,
    42350, 59750,
    42636, 59914,
    42923, 60075,
    43210, 60235,
    43500, 60392,
    43790, 60547,
    44081, 60700,
    44374, 60851,
    44668, 60999,
    44963, 61145,
    45259, 61288,
    45557, 61429,
    45856, 61568,
    46156, 61705,
    46457, 61839,
    46760, 61971,
    47064, 62101,
    47369, 62228,
    47676, 62353,
    47984, 62476,
    48294, 62596,
    48605, 62714,
    48917, 62830,
    49231, 62943,
    49546, 63054,
    49863, 63162,
    50181, 63268,
    50501, 63372,
    50822, 63473,
    51145, 63572,
    51469, 63668,
    51795, 63763,
    52122, 63854,
    52451, 63944,
    52782, 64031,
    53114, 64115,
    53448, 64197,
    53784, 64277,
    54121, 64354,
    54460, 64429,
    54801, 64501,
    55144, 64571,
    55488, 64639,
    55834, 64704,
    56182, 64766,
    56532, 64827,
    56883, 64884,
    57237, 64940,
    57592, 64993,
    57950, 65043,
    58309, 65091,
    58670, 65137,
    59033, 65180,
    59398, 65220,
    59766, 65259,
    60135, 65294,
    60506, 65328,
    60880, 65358,
    61255, 65387,
    61633, 65413,
    62013, 65436,
    62395, 65457,
    62780, 65476,
    63167, 65492,
    63556, 65505,
    63947, 65516,
    64341, 65525,
    64737, 65531,
    65135, 65535,
};

void blit_trishear_pow2_tex_internal(uint16_t* fb, int f_xs, uint16_t* tex, int t_xs_log2, int t_ys, int x, int y, int xsr, int ysr, int xsr2, const int flip) {
    int32_t cxsr = 0;
    int t_xs = (1u << t_xs_log2);
    int32_t t_pos = (flip) ? (t_xs * t_ys - 1) : 0;
    int32_t fb_pos = (y * f_xs + x);
    int32_t yshift = 0;
    int32_t xshift2 = 0;
    //mp_printf(&mp_plat_print, "Rastering %i, %i, with (xsr, ysr, xsr2) (%f, %f, %f)", t_xs, t_ys, xsr/65536.f, ysr/65536.f, xsr2/65536.f);
    const int t_inc = (flip) ? -1 : 1;
    for(int cy = 0; cy < t_ys; cy++) {
        yshift = (cxsr >> 16) * ysr;
        fb_pos += (cxsr >> 16);
        for(int cx = 0; cx < t_xs; cx++) {
            xshift2 = ((cy + (yshift >> 16)) * xsr2);

            fb[fb_pos + (cx) + (yshift >> 16) * f_xs + (xshift2 >> 16)] = tex[t_pos];

            t_pos += t_inc;
            yshift += ysr;
        }
        fb_pos -= (cxsr >> 16);
        cxsr += xsr;
        fb_pos += f_xs;
    }
}

// Theta is a 10-bit brad angle. 0..2pi is scaled to 0..1024
void blit_rotate_pow2_tex_internal(uint16_t* fb, int f_xs, uint16_t* tex, int t_xs_log2, int t_ys, const int x, const int y, int16_t theta) {
    int flip = 0;
    int32_t t_xs = 1u << t_xs_log2;
    // Step 1: Get theta inside (-pi/2, pi/2) and flip if we need to
    theta &= 0x3FF;
    if(theta > 0x200) theta -= 0x400;
    if(theta > 0x100) {
        flip = 1;
        theta -= 0x200;
    } else if(theta < -0x100) {
        flip = 1;
        theta += 0x200;
    }

    int negative = 0;
    if(theta < 0) {
        negative = 1;
        theta = -theta;
    }

    //mp_printf(&mp_plat_print, "Converted theta to %f\n\r", theta * 3.14159265 / 256.f);

    int idx = (theta << 1);
    //printf("Table index is %i\n\r", idx);
    int32_t a, b; // tan(theta*0.5), sin(theta)
    if(idx != 512) {
        a = (negative) ? sin_tan_tab[idx] : -sin_tan_tab[idx];
        b = (negative) ? -sin_tan_tab[idx+1] : sin_tan_tab[idx+1];
    } else {
        a = (negative) ? 65536 : -65536;
        b = (negative) ? -65536 : 65536;
    }
    int32_t c = (((int64_t)a*b) >> 16) + 0x10000;
    //printf("tan(0.5t)=%f, sin(t)=%f\n\r", a/65536.f, b/65536.f);
    //printf("Flip: %i\n\r", flip);
    // Step 3: Rotate center w.r.t. pivot so we can rotate about the center instead
    int cx = ((int64_t)(t_xs/2) * c - (int64_t)(t_ys/2) * b) >> 16;
    int cy = ((int64_t)(t_ys/2) * c + (int64_t)(t_xs/2) * b) >> 16;
    //Step 4: Triple shear (a, b, a);
    blit_trishear_pow2_tex_internal(fb, f_xs, tex, t_xs_log2, t_ys, x - cx, y - cy, a, b, a, flip);
}

void blit_scale_pow2_tex_internal(uint16_t* fb, int f_xs, uint16_t* tex, int t_xs_log2, int t_ys, int32_t x, int32_t y, int32_t xsc, const int32_t ysc) {
    int32_t t_xs = 1u << t_xs_log2;
    int32_t xe = (t_xs * xsc) >> 16;
    int32_t ye = (t_ys * ysc) >> 16;
    int32_t dtx = ((int64_t)t_xs << 16) / xe;
    int32_t dty = ((int64_t)t_ys << 16) / ye;
    int32_t ty = 0x8000;
    int32_t tx = 0x8000;
    if(xsc < 0) {
        xe = -xe;
        x -= xe;
    }
    if(ysc < 0) {
        ye = -ye;
        y -= ye;
        ty = (t_ys << 16) - 0x8000;
    }
    //printf("dtx=%i, dty=%i\n\r", dtx, dty);
    int32_t fb_pos = y * f_xs + x;
    for(int cy = 0; cy < ye; cy++) {
        tx = (xsc < 0) ? ((t_xs << 16) - 0x8000) : 0x8000;
        for(int cx = 0; cx < xe; cx++) {

            fb[fb_pos + cx] = tex[((ty >> 16) << t_xs_log2) + (tx >> 16)];

            tx += dtx;
        }
        fb_pos += f_xs;
        ty += dty;
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

STATIC mp_obj_t blit_trishear_pow2_tex(mp_uint_t n_args, const mp_obj_t* args)
{
    mp_obj_t fb = args[0];
    mp_buffer_info_t buf_inf;
    mp_get_buffer_raise(fb, &buf_inf, MP_BUFFER_WRITE);
    int f_xs = mp_obj_get_int(args[1]);
    //int f_ys = mp_obj_get_int(args[2]);
    mp_obj_t tex = args[3];
    mp_buffer_info_t tex_inf;
    mp_get_buffer_raise(tex, &tex_inf, MP_BUFFER_READ);
    int t_xsl2 = mp_obj_get_int(args[4]);
    int t_ys = mp_obj_get_int(args[5]);
    const int x = MP_OBJ_SMALL_INT_VALUE(args[6]);
    const int y = MP_OBJ_SMALL_INT_VALUE(args[7]);
    const int xsr = MP_OBJ_SMALL_INT_VALUE(args[8]);
    const int ysr = MP_OBJ_SMALL_INT_VALUE(args[9]);
    const int xsr2 = MP_OBJ_SMALL_INT_VALUE(args[10]);
    const int flip = MP_OBJ_SMALL_INT_VALUE(args[11]);
    //mp_printf(&mp_plat_print, "Rastering %i, %i, with (xsr, ysr, xsr2) (%f, %f, %f)", t_xs, t_ys, xsr/65536.f, ysr/65536.f, xsr2/65536.f);
    //init_interp(t_xs);
    blit_trishear_pow2_tex_internal(buf_inf.buf, f_xs, tex_inf.buf, t_xsl2, t_ys, x, y, xsr, ysr, xsr2, flip);
    return mp_const_none;
}
MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(blit_trishear_pow2_tex_obj, 12, 12, blit_trishear_pow2_tex);

STATIC mp_obj_t blit_rotate_pow2_tex(mp_uint_t n_args, const mp_obj_t* args)
{
    mp_obj_t fb = args[0];
    mp_buffer_info_t buf_inf;
    mp_get_buffer_raise(fb, &buf_inf, MP_BUFFER_WRITE);
    int f_xs = mp_obj_get_int(args[1]);
    //int f_ys = mp_obj_get_int(args[2]);
    mp_obj_t tex = args[3];
    mp_buffer_info_t tex_inf;
    mp_get_buffer_raise(tex, &tex_inf, MP_BUFFER_READ);
    int t_xsl2 = mp_obj_get_int(args[4]);
    int t_ys = mp_obj_get_int(args[5]);
    const int x = MP_OBJ_SMALL_INT_VALUE(args[6]);
    const int y = MP_OBJ_SMALL_INT_VALUE(args[7]);
    const int theta = MP_OBJ_SMALL_INT_VALUE(args[8]);
    //mp_printf(&mp_plat_print, "Rastering %i, %i, with (xsr, ysr, xsr2) (%f, %f, %f)", t_xs, t_ys, xsr/65536.f, ysr/65536.f, xsr2/65536.f);
    //init_interp(t_xs);
    blit_rotate_pow2_tex_internal(buf_inf.buf, f_xs, tex_inf.buf, t_xsl2, t_ys, x, y, theta);
    return mp_const_none;
}
MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(blit_rotate_pow2_tex_obj, 9, 9, blit_rotate_pow2_tex);

STATIC mp_obj_t blit_scale_pow2_tex(mp_uint_t n_args, const mp_obj_t* args)
{
    mp_obj_t fb = args[0];
    mp_buffer_info_t buf_inf;
    mp_get_buffer_raise(fb, &buf_inf, MP_BUFFER_WRITE);
    int f_xs = mp_obj_get_int(args[1]);
    //int f_ys = mp_obj_get_int(args[2]);
    mp_obj_t tex = args[3];
    mp_buffer_info_t tex_inf;
    mp_get_buffer_raise(tex, &tex_inf, MP_BUFFER_READ);
    int t_xsl2 = mp_obj_get_int(args[4]);
    int t_ys = mp_obj_get_int(args[5]);
    const int x = MP_OBJ_SMALL_INT_VALUE(args[6]);
    const int y = MP_OBJ_SMALL_INT_VALUE(args[7]);
    const int xsc = MP_OBJ_SMALL_INT_VALUE(args[8]);
    const int ysc = MP_OBJ_SMALL_INT_VALUE(args[9]);
    //mp_printf(&mp_plat_print, "Rastering %i, %i, with (xsr, ysr, xsr2) (%f, %f, %f)", t_xs, t_ys, xsr/65536.f, ysr/65536.f, xsr2/65536.f);
    //init_interp(t_xs);
    blit_scale_pow2_tex_internal(buf_inf.buf, f_xs, tex_inf.buf, t_xsl2, t_ys, x, y, xsc, ysc);
    return mp_const_none;
}
MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(blit_scale_pow2_tex_obj, 10, 10, blit_scale_pow2_tex);

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
    int t_ys = mp_obj_get_int(args[5]);
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
    { MP_ROM_QSTR(MP_QSTR_blit_trishear_pow2_tex), MP_ROM_PTR(&blit_trishear_pow2_tex_obj) },
    { MP_ROM_QSTR(MP_QSTR_blit_rotate_pow2_tex), MP_ROM_PTR(&blit_rotate_pow2_tex_obj) },
    { MP_ROM_QSTR(MP_QSTR_blit_scale_pow2_tex), MP_ROM_PTR(&blit_scale_pow2_tex_obj) },
};

STATIC MP_DEFINE_CONST_DICT(mp_module_raster_globals, mp_module_raster_globals_table);

const mp_obj_module_t mp_module_raster = {
    .base = { &mp_type_module },
    .globals = (mp_obj_dict_t *)&mp_module_raster_globals,
};

MP_REGISTER_MODULE(MP_QSTR_raster, mp_module_raster);
