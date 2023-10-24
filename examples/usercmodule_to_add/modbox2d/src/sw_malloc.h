/* Miserable malloc - the (almost) MISRA-compliant, portable, C89- / ANSI C- compatible heap allocator. */

/*
 * This file is part of Miserable Malloc.
 *
 * The MIT License (MIT)
 *
 * Copyright (c) 2023 Mason Watmough
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

#pragma once

#include <stdint.h>
#include <stdbool.h>
#include "py/mpprint.h"

#define KB *(1024u)
#define MB *(1024u KB)

#define MEMORY_SIZE (48u KB) /* Note: memory sizes > 2GB are not supported in ANSI C / C89 due to long long integer requirements */
#define MEMORY_ALIGNMENT (16u)  /* 0 for unaligned */
#define ALLOC_CLEAR_ON_INIT /* If defined, the entire memory space is cleared on initialization */
#define ALLOC_CLEAR_VALUE (0xF3u)
#define MAX_CONCAT_SEARCH_DISTANCE (8u)  /* How far to look when concatenating blocks */
#define MAX_LFB_SEARCH_DISTANCE (8u)  /* How far to look for lowest free block */

#define ALLOC_LOW_PADDING (1u)  /* How many bytes to pad allocations below a returned pointer. This can be a function of TAGSIZE. */
#define ALLOC_HIGH_PADDING (2u)  /* How many bytes to pad allocations above a returned pointer. This can be a function of TAGSIZE. */
/* #define LAZY_INIT */ /* Initialize allocator only as-needed? */

/* #define GUARDRAILS (0) */ /* No protections whatsoever. Does not include assert.h. */
/* #define GUARDRAILS (1) */ /* Out-of-memory checks, double-free checks, and block corruption checks for out-of-bounds / "segmentation fault" detection. */
/* #define GUARDRAILS (2) */ /* GUARDRAILS == 1 plus block concatenation validity checks and pointer validity checks. */
/* #define GUARDRAILS (3) */ /* GUARDRAILS == 2 plus CRC8 error checking to detect use-after-free; Somewhat expensive. */

#define GUARDRAILS (3)

/* #define DEBUG */

#ifdef DEBUG /* Use real debug-print functions for ISO C pedantry regarding macro varargs */
  #include <stdio.h>
  #include <stdarg.h>
  static void dbgprint(const char* fmt, ...) {va_list args; va_start(args, fmt); mp_printf(MP_PYTHON_PRINTER, fmt, args); va_end(args);}
#else
  static void dbgprint(const char* fmt, ...) {(void)fmt; do {} while(0);}
#endif

#if (GUARDRAILS > 2)
  #define USE_ERROR_DETECTION
#endif

#if (GUARDRAILS > 0) || defined(USE_ERROR_DETECTION)
  #include <assert.h>
#endif

#if (ALLOC_LOW_PADDING <= 0)
  #pragma message("Allocator does not use lower padding, be wary of block corruption due to out-of-bounds writes!")
#endif
#if (ALLOC_HIGH_PADDING <= 0)
  #pragma message("Allocator does not use upper padding, be wary of block corruption due to out-of-bounds writes!")
#endif

#if !(__STRICT_ANSI__) && (__STDC_VERSION__ > 199409L) && (MEMORY_ALIGNMENT > 0)

  #define MEMORY_ALIGNMENT_ATTRIBUTE __attribute__((aligned(MEMORY_ALIGNMENT)))
  #define INLINE_FUNCTION inline __attribute__((always_inline))

#else

  #if (__STRICT_ANSI__) || (__STDC_VERSION__ <= 199409L)
  #if (MEMORY_SIZE > 2048u MB)
  #error "Memory sizes > 2GB are not supported in ANSI C / C89 mode."
  #endif
  #pragma message("ANSI C / C89 detected, no memory alignment or inlining!")
  #endif

  #define MEMORY_ALIGNMENT_ATTRIBUTE
  #define INLINE_FUNCTION

#endif

static MEMORY_ALIGNMENT_ATTRIBUTE uint8_t _memory_space[MEMORY_SIZE];

#if MEMORY_SIZE <= (0x80u)
  typedef uint8_t TAG;
  #define TAG_FREE_BIT (0x80u)
#elif MEMORY_SIZE <= (0x8000u)
  typedef uint16_t TAG;
  #define TAG_FREE_BIT (0x8000u)
#elif MEMORY_SIZE <= (0x80000000u)
  typedef uint32_t TAG;
  #define TAG_FREE_BIT (0x80000000u)
#else
  typedef uint64_t TAG;
  #define TAG_FREE_BIT (0x8000000000000000u)
#endif

#ifdef USE_ERROR_DETECTION
  #define TAGSIZE (unsigned int)(sizeof(TAG) + 1u) /* CRC8 takes extra byte */
#else
  #define TAGSIZE (unsigned int)(sizeof(TAG))
#endif

#define TAG_FREE(T) (((T)&TAG_FREE_BIT) != 0u)
#define GET_SIZE(T) ((T) & (~TAG_FREE_BIT))

#define GUARDRAIL_ASSERT(cond, name) (name) = (cond); assert(name)

static INLINE_FUNCTION TAG ptr_to_offset(const uint8_t* p) {
  return (TAG)((ptrdiff_t)(&(p)[-(ptrdiff_t)_memory_space])); /* Last remaining MISRA violation. I don't know how to calculate this pointer offset in a compliant way. */
}

#ifdef USE_ERROR_DETECTION

  #define CRC_STEP_SIZE (1u) /* Making this larger speeds up the CRC calculation but makes it less robust at error detection */
  #define CRC_MAX_SIZE (256u) /* Prevent extremely large CRCs */

uint8_t crc8(uint8_t crc, uint8_t const* mem, size_t len) {

#if ((CRC_MAX_SIZE / CRC_STEP_SIZE) > 32u) /* Should we use a CRC optimized for long data? */
  static uint8_t const crc8_table[256] = { /* CRC-8 with polynomial 0xA6 */
    0x00, 0x3C, 0x78, 0x44, 0x3B, 0x07, 0x43, 0x7F, 0x76, 0x4A, 0x0E, 0x32, 0x4D, 0x71, 0x35, 0x09, 0x27, 0x1B, 0x5F, 0x63, 0x1C, 0x20, 0x64, 0x58, 0x51, 0x6D, 0x29, 0x15, 0x6A, 0x56, 0x12, 0x2E,
    0x4E, 0x72, 0x36, 0x0A, 0x75, 0x49, 0x0D, 0x31, 0x38, 0x04, 0x40, 0x7C, 0x03, 0x3F, 0x7B, 0x47, 0x69, 0x55, 0x11, 0x2D, 0x52, 0x6E, 0x2A, 0x16, 0x1F, 0x23, 0x67, 0x5B, 0x24, 0x18, 0x5C, 0x60,
    0x57, 0x6B, 0x2F, 0x13, 0x6C, 0x50, 0x14, 0x28, 0x21, 0x1D, 0x59, 0x65, 0x1A, 0x26, 0x62, 0x5E, 0x70, 0x4C, 0x08, 0x34, 0x4B, 0x77, 0x33, 0x0F, 0x06, 0x3A, 0x7E, 0x42, 0x3D, 0x01, 0x45, 0x79,
    0x19, 0x25, 0x61, 0x5D, 0x22, 0x1E, 0x5A, 0x66, 0x6F, 0x53, 0x17, 0x2B, 0x54, 0x68, 0x2C, 0x10, 0x3E, 0x02, 0x46, 0x7A, 0x05, 0x39, 0x7D, 0x41, 0x48, 0x74, 0x30, 0x0C, 0x73, 0x4F, 0x0B, 0x37,
    0x65, 0x59, 0x1D, 0x21, 0x5E, 0x62, 0x26, 0x1A, 0x13, 0x2F, 0x6B, 0x57, 0x28, 0x14, 0x50, 0x6C, 0x42, 0x7E, 0x3A, 0x06, 0x79, 0x45, 0x01, 0x3D, 0x34, 0x08, 0x4C, 0x70, 0x0F, 0x33, 0x77, 0x4B,
    0x2B, 0x17, 0x53, 0x6F, 0x10, 0x2C, 0x68, 0x54, 0x5D, 0x61, 0x25, 0x19, 0x66, 0x5A, 0x1E, 0x22, 0x0C, 0x30, 0x74, 0x48, 0x37, 0x0B, 0x4F, 0x73, 0x7A, 0x46, 0x02, 0x3E, 0x41, 0x7D, 0x39, 0x05,
    0x32, 0x0E, 0x4A, 0x76, 0x09, 0x35, 0x71, 0x4D, 0x44, 0x78, 0x3C, 0x00, 0x7F, 0x43, 0x07, 0x3B, 0x15, 0x29, 0x6D, 0x51, 0x2E, 0x12, 0x56, 0x6A, 0x63, 0x5F, 0x1B, 0x27, 0x58, 0x64, 0x20, 0x1C,
    0x7C, 0x40, 0x04, 0x38, 0x47, 0x7B, 0x3F, 0x03, 0x0A, 0x36, 0x72, 0x4E, 0x31, 0x0D, 0x49, 0x75, 0x5B, 0x67, 0x23, 0x1F, 0x60, 0x5C, 0x18, 0x24, 0x2D, 0x11, 0x55, 0x69, 0x16, 0x2A, 0x6E, 0x52};
#else
  static uint8_t const crc8_table[256] = { /* CRC-8 with polynomial 0x97 */
    0x00, 0x45, 0x8A, 0xCF, 0xC7, 0x82, 0x4D, 0x08, 0x5D, 0x18, 0xD7, 0x92, 0x9A, 0xDF, 0x10, 0x55, 0xBA, 0xFF, 0x30, 0x75, 0x7D, 0x38, 0xF7, 0xB2, 0xE7, 0xA2, 0x6D, 0x28, 0x20, 0x65, 0xAA, 0xEF,
    0xA7, 0xE2, 0x2D, 0x68, 0x60, 0x25, 0xEA, 0xAF, 0xFA, 0xBF, 0x70, 0x35, 0x3D, 0x78, 0xB7, 0xF2, 0x1D, 0x58, 0x97, 0xD2, 0xDA, 0x9F, 0x50, 0x15, 0x40, 0x05, 0xCA, 0x8F, 0x87, 0xC2, 0x0D, 0x48,
    0x9D, 0xD8, 0x17, 0x52, 0x5A, 0x1F, 0xD0, 0x95, 0xC0, 0x85, 0x4A, 0x0F, 0x07, 0x42, 0x8D, 0xC8, 0x27, 0x62, 0xAD, 0xE8, 0xE0, 0xA5, 0x6A, 0x2F, 0x7A, 0x3F, 0xF0, 0xB5, 0xBD, 0xF8, 0x37, 0x72,
    0x3A, 0x7F, 0xB0, 0xF5, 0xFD, 0xB8, 0x77, 0x32, 0x67, 0x22, 0xED, 0xA8, 0xA0, 0xE5, 0x2A, 0x6F, 0x80, 0xC5, 0x0A, 0x4F, 0x47, 0x02, 0xCD, 0x88, 0xDD, 0x98, 0x57, 0x12, 0x1A, 0x5F, 0x90, 0xD5,
    0xE9, 0xAC, 0x63, 0x26, 0x2E, 0x6B, 0xA4, 0xE1, 0xB4, 0xF1, 0x3E, 0x7B, 0x73, 0x36, 0xF9, 0xBC, 0x53, 0x16, 0xD9, 0x9C, 0x94, 0xD1, 0x1E, 0x5B, 0x0E, 0x4B, 0x84, 0xC1, 0xC9, 0x8C, 0x43, 0x06,
    0x4E, 0x0B, 0xC4, 0x81, 0x89, 0xCC, 0x03, 0x46, 0x13, 0x56, 0x99, 0xDC, 0xD4, 0x91, 0x5E, 0x1B, 0xF4, 0xB1, 0x7E, 0x3B, 0x33, 0x76, 0xB9, 0xFC, 0xA9, 0xEC, 0x23, 0x66, 0x6E, 0x2B, 0xE4, 0xA1,
    0x74, 0x31, 0xFE, 0xBB, 0xB3, 0xF6, 0x39, 0x7C, 0x29, 0x6C, 0xA3, 0xE6, 0xEE, 0xAB, 0x64, 0x21, 0xCE, 0x8B, 0x44, 0x01, 0x09, 0x4C, 0x83, 0xC6, 0x93, 0xD6, 0x19, 0x5C, 0x54, 0x11, 0xDE, 0x9B,
    0xD3, 0x96, 0x59, 0x1C, 0x14, 0x51, 0x9E, 0xDB, 0x8E, 0xCB, 0x04, 0x41, 0x49, 0x0C, 0xC3, 0x86, 0x69, 0x2C, 0xE3, 0xA6, 0xAE, 0xEB, 0x24, 0x61, 0x34, 0x71, 0xBE, 0xFB, 0xF3, 0xB6, 0x79, 0x3C};
#endif

  uint8_t const* data = mem;
  uint8_t ret = crc;
  size_t remaining = len;

  if(remaining > CRC_MAX_SIZE) {remaining = CRC_MAX_SIZE;}

  dbgprint("%u-byte crc at %u\n\r", (int)remaining, ptr_to_offset(mem));
  if ((data == NULL) || (remaining < (size_t)CRC_STEP_SIZE)) {
    ret = (uint8_t)0xff;
  } else {
    TAG pos = 0;
    while ((remaining-=CRC_STEP_SIZE) >= (size_t)CRC_STEP_SIZE) {
      ret = crc8_table[crc ^ data[pos]];
      pos += CRC_STEP_SIZE;
    }
  }
  return ret;
}

#endif

#if defined(__BYTE_ORDER__) && (__BYTE_ORDER__ == __ORDER_BIG_ENDIAN__) /* Work out (guess) platform endianness; Replace pointer type-punning with explicit unsigned read and write */
#pragma message("Using BE tags")
static INLINE_FUNCTION void write_tag(const TAG offset, const TAG t) {
  #if MEMORY_SIZE <= (0x80u)
    _memory_space[offset] = (uint8_t)(t);
  #elif MEMORY_SIZE <= (0x8000u)
    _memory_space[offset] = (uint8_t)(t);
    _memory_space[offset+1u] = (uint8_t)(t >> 8);
  #elif MEMORY_SIZE <= (0x80000000u)
    _memory_space[offset] = (uint8_t)(t);
    _memory_space[offset+1u] = (uint8_t)(t >> 8);
    _memory_space[offset+2u] = (uint8_t)(t >> 16);
    _memory_space[offset+3u] = (uint8_t)(t >> 24);
  #else
    _memory_space[offset] = (uint8_t)(t);
    _memory_space[offset+1u] = (uint8_t)(t >> 8);
    _memory_space[offset+2u] = (uint8_t)(t >> 16);
    _memory_space[offset+3u] = (uint8_t)(t >> 24);
    _memory_space[offset+4u] = (uint8_t)(t >> 32);
    _memory_space[offset+5u] = (uint8_t)(t >> 40);
    _memory_space[offset+6u] = (uint8_t)(t >> 48);
    _memory_space[offset+7u] = (uint8_t)(t >> 56);
  #endif
}
static INLINE_FUNCTION TAG get_tag(const TAG offset) {
  TAG ret = 0;
  #if MEMORY_SIZE <= (0x80u)
    ret = (TAG)(_memory_space[offset]);
  #elif MEMORY_SIZE <= (0x8000u)
    ret |= (TAG)(_memory_space[offset]);
    ret |= (TAG)(_memory_space[offset+1u]) << 8;
  #elif MEMORY_SIZE <= (0x80000000u)
    ret |= (TAG)(_memory_space[offset]);
    ret |= (TAG)(_memory_space[offset+1u]) << 8;
    ret |= (TAG)(_memory_space[offset+2u]) << 16;
    ret |= (TAG)(_memory_space[offset+3u]) << 24;
  #else
    ret |= (TAG)(_memory_space[offset]);
    ret |= (TAG)(_memory_space[offset+1u]) << 8;
    ret |= (TAG)(_memory_space[offset+2u]) << 16;
    ret |= (TAG)(_memory_space[offset+3u]) << 24;
    ret |= (TAG)(_memory_space[offset+4u]) << 32;
    ret |= (TAG)(_memory_space[offset+5u]) << 40;
    ret |= (TAG)(_memory_space[offset+6u]) << 48;
    ret |= (TAG)(_memory_space[offset+7u]) << 56;
  #endif
  return ret;
}
#else /* Assume little-endian */

#pragma message("Using LE tags")
static INLINE_FUNCTION void write_tag(const TAG offset, const TAG t) {
  #if MEMORY_SIZE <= (0x80u)
    _memory_space[offset] = (uint8_t)(t);
  #elif MEMORY_SIZE <= (0x8000u)
    _memory_space[offset] = (uint8_t)(t >> 8);
    _memory_space[offset+1u] = (uint8_t)(t);
  #elif MEMORY_SIZE <= (0x80000000u)
    _memory_space[offset] = (uint8_t)(t >> 24);
    _memory_space[offset+1u] = (uint8_t)(t >> 16);
    _memory_space[offset+2u] = (uint8_t)(t >> 8);
    _memory_space[offset+3u] = (uint8_t)(t);
  #else
    _memory_space[offset] = (uint8_t)(t >> 56);
    _memory_space[offset+1u] = (uint8_t)(t >> 48);
    _memory_space[offset+2u] = (uint8_t)(t >> 40);
    _memory_space[offset+3u] = (uint8_t)(t >> 32);
    _memory_space[offset+4u] = (uint8_t)(t >> 24);
    _memory_space[offset+5u] = (uint8_t)(t >> 16);
    _memory_space[offset+6u] = (uint8_t)(t >> 8);
    _memory_space[offset+7u] = (uint8_t)(t);
  #endif
}
static INLINE_FUNCTION TAG get_tag(const TAG offset) {
  TAG ret = 0;
  #if MEMORY_SIZE <= (0x80u)
    ret = (TAG)(_memory_space[offset]);
  #elif MEMORY_SIZE <= (0x8000u)
    ret |= (TAG)(_memory_space[offset]) << 8;
    ret |= (TAG)(_memory_space[offset+1u]);
  #elif MEMORY_SIZE <= (0x80000000u)
    ret |= (TAG)(_memory_space[offset]) << 24;
    ret |= (TAG)(_memory_space[offset+1u]) << 16;
    ret |= (TAG)(_memory_space[offset+2u]) << 8;
    ret |= (TAG)(_memory_space[offset+3u]);
  #else
    ret |= (TAG)(_memory_space[offset]) << 56;
    ret |= (TAG)(_memory_space[offset+1u]) << 48;
    ret |= (TAG)(_memory_space[offset+2u]) << 40;
    ret |= (TAG)(_memory_space[offset+3u]) << 32;
    ret |= (TAG)(_memory_space[offset+4u]) << 24;
    ret |= (TAG)(_memory_space[offset+5u]) << 16;
    ret |= (TAG)(_memory_space[offset+6u]) << 8;
    ret |= (TAG)(_memory_space[offset+7u]);
  #endif
  return ret;
}
#endif

static INLINE_FUNCTION TAG get_back_tag(const TAG offset) {
  return get_tag(offset-TAGSIZE);
}

static INLINE_FUNCTION TAG get_front_tag(const TAG offset) {
  return get_tag(offset+GET_SIZE(get_back_tag(offset)));
}

static INLINE_FUNCTION void write_back_tag(const TAG offset, const TAG t) {
  write_tag(offset-TAGSIZE, t);
}

static INLINE_FUNCTION void write_front_tag(const TAG offset, const TAG t) {
  write_tag(offset+GET_SIZE(get_back_tag(offset)), t);
}

static INLINE_FUNCTION TAG get_prev_tag(const TAG offset) {
  return (offset >= (2u*TAGSIZE)) ? get_tag(offset - (2u * TAGSIZE)) : (TAG)0;
}

static INLINE_FUNCTION TAG get_next_tag(const TAG offset) {
  return get_tag(offset+GET_SIZE(get_back_tag(offset)) + TAGSIZE);
}

static INLINE_FUNCTION void write_tags(const TAG offset, const TAG t) {
  write_back_tag(offset, t);
  write_front_tag(offset, t);
}

#ifdef USE_ERROR_DETECTION

  static INLINE_FUNCTION void write_crcs(const TAG offset, const uint8_t crc) {
    (*(&_memory_space[offset - 1u])) = crc;
    (*(&_memory_space[offset + GET_SIZE(get_back_tag(offset)) + TAGSIZE - 1u])) = crc;
  }

  static INLINE_FUNCTION bool crc_verify(const TAG offset) {
    const bool ret = (crc8(0xff, &_memory_space[offset], GET_SIZE(get_back_tag(offset))) == *(&_memory_space[offset - 1u]));
    return ret;
  }

  static INLINE_FUNCTION bool write_crcs_verify(const TAG offset, const uint8_t crc) {
    const bool ret = (crc == *(&_memory_space[offset - 1u]));
    (*(&_memory_space[offset - 1u])) = crc;
    (*(&_memory_space[offset + GET_SIZE(get_back_tag(offset)) + TAGSIZE - 1u])) = crc;
    return ret;
  }

#endif

static TAG heap_top = 0;
static TAG lfb_pos = 0;
static TAG lfb_size = 0;

#ifndef LAZY_INIT
  #if defined(_MSC_VER)
    #pragma startup alloc_init
  #elif defined(__GNUC__) || defined(__clang__)
    static void __attribute__((constructor)) alloc_init(void);
  #else
    #pragma message("Compiler does not support initialization before main(), using LAZY_INIT")
    #define LAZY_INIT
  #endif
#endif

/* Allocator initialization */
static void alloc_init(void) {

  #ifdef ALLOC_CLEAR_ON_INIT
    TAG p = (TAG)TAGSIZE;
  #endif

  #if (GUARDRAILS > 0u)
    bool REASONABLE_MEMORY_SIZE;
    GUARDRAIL_ASSERT((MEMORY_SIZE > (4u * TAGSIZE)), REASONABLE_MEMORY_SIZE);
  #endif

  heap_top = (TAG)TAGSIZE;
  lfb_pos = (TAG)TAGSIZE;
  lfb_size = (TAG)(MEMORY_SIZE - (2u * TAGSIZE));
  write_tags(heap_top, lfb_size | TAG_FREE_BIT);

  #ifdef ALLOC_CLEAR_ON_INIT
    dbgprint("Clearing with %i\n\r", (int)ALLOC_CLEAR_VALUE);
    for(; p < MEMORY_SIZE - TAGSIZE; p++)
    {
      _memory_space[p] = ALLOC_CLEAR_VALUE;
    }
  #endif

  #ifdef USE_ERROR_DETECTION
     write_crcs(lfb_pos, crc8(0xff, &_memory_space[TAGSIZE], (MEMORY_SIZE - (2u * TAGSIZE))));
  #endif

  dbgprint("Allocator initialized\n\r");
}

/* Internal functions for re-grouping freed and allocated blocks */
static INLINE_FUNCTION void _concatenate_next(const TAG t) {
  /* Concatenate a block with the block ahead of it */
  #if (GUARDRAILS > 1)
    bool F_CONCAT_VALIDITY;
  #endif

  const TAG new_size = GET_SIZE(get_back_tag(t)) + GET_SIZE(get_next_tag(t)) + (2u * TAGSIZE);

  #ifdef USE_ERROR_DETECTION
    bool CRC_ERROR_DETECTION_IN_REALLOC; /* this function is only called from _padless_realloc */
    GUARDRAIL_ASSERT(crc_verify(t + GET_SIZE(get_back_tag(t)) + (2u * TAGSIZE)), CRC_ERROR_DETECTION_IN_REALLOC);
  #endif

  #if (GUARDRAILS > 1)
    GUARDRAIL_ASSERT(((t + new_size) <= (MEMORY_SIZE - (1u * TAGSIZE))), F_CONCAT_VALIDITY);
  #endif

  write_tags(t, new_size);
}

static INLINE_FUNCTION void _concatenate_free_next(const TAG t) {
  /* Concatenate a freed block with the block ahead of it */
  #if (GUARDRAILS > 1)
    bool F_CONCAT_VALIDITY;
  #endif

  const TAG new_size = GET_SIZE(get_back_tag(t)) + GET_SIZE(get_next_tag(t)) + (2u * TAGSIZE);

  #ifdef USE_ERROR_DETECTION
    bool CRC_ERROR_DETECTION_IN_FREE; /* this function is only called from _padless_free */
    GUARDRAIL_ASSERT(crc_verify(t + GET_SIZE(get_back_tag(t)) + (2u * TAGSIZE)), CRC_ERROR_DETECTION_IN_FREE);
  #endif

  #if (GUARDRAILS > 1)
    GUARDRAIL_ASSERT(((t + new_size) <= (MEMORY_SIZE - (1u * TAGSIZE))), F_CONCAT_VALIDITY);
  #endif

  write_tags(t, new_size | TAG_FREE_BIT);
}

static INLINE_FUNCTION TAG _concatenate_free_prev(TAG t) {
  /* Concatenate a freed block with the block behind it and return the new starting offset */
  TAG ret = t;
  if(t != TAGSIZE) { /* This is the only concatenation that seems to need explicit guarding for this */

  #if (GUARDRAILS > 1)
    bool B_CONCAT_VALIDITY;
  #endif

  const TAG new_size = GET_SIZE(get_back_tag(t)) + GET_SIZE(get_prev_tag(t)) + (2u * TAGSIZE);

  #ifdef USE_ERROR_DETECTION
    bool CRC_ERROR_DETECTION_IN_FREE; /* this function is only called from _padless_free */
    GUARDRAIL_ASSERT(crc_verify(t - GET_SIZE(get_prev_tag(t)) - (2u * TAGSIZE)), CRC_ERROR_DETECTION_IN_FREE);
  #endif

  ret -= (GET_SIZE(get_prev_tag(ret)) + (2u * TAGSIZE));

  #if (GUARDRAILS > 1)
    GUARDRAIL_ASSERT(((t >= TAGSIZE) && (t <= (MEMORY_SIZE - TAGSIZE))), B_CONCAT_VALIDITY);
  #endif

  write_tags(ret, new_size | TAG_FREE_BIT);
  }
  return ret; /* Return new offset */
}

static INLINE_FUNCTION void _lfb_search_inclusive(const TAG start, const unsigned int max_checks) {
  /* Search for the lowest free block, potentially including the starting position */
  TAG check = start;
  unsigned int checks = 0;
  bool found = false;
  dbgprint("Searching for lfb at %u...\n\r", start);

  while ((checks < max_checks) && (check > TAGSIZE) && !found) {
    check -= (GET_SIZE(get_prev_tag(check)) + (2u * TAGSIZE));
    if ((check >= (2u * TAGSIZE)) && (check < (MEMORY_SIZE - (2u * TAGSIZE))) && TAG_FREE(get_back_tag(check))) {
      lfb_pos = check;
      lfb_size = GET_SIZE(get_back_tag(check));
      dbgprint("new lfb at %u \n\r", lfb_pos);
      found = true;
    } else {break;}
    checks++;
  }
  checks = 0;
  check = start;

  while ((checks < max_checks) && !found) {
    check += GET_SIZE(get_back_tag(check)) + (2u * TAGSIZE);
    dbgprint("checking %u \n\r", (int)check);
    if ((check < (MEMORY_SIZE - TAGSIZE)) && TAG_FREE(get_back_tag(check))) {
      lfb_pos = check;
      lfb_size = GET_SIZE(get_back_tag(check));
      dbgprint("new lfb at %u \n\r", lfb_pos);
      found = true;
    } else {break;}
    checks++;
  }
  if(!found) {lfb_pos = start;}
  dbgprint("Finished search\n\r");
}

static INLINE_FUNCTION void _lfb_search(const TAG start, const int max_checks) {
  /* Look for a new lowest free block, excluding the starting position */
  _lfb_search_inclusive(start, max_checks);
  if (lfb_pos == start) {
    lfb_pos = heap_top; /* Give up */
    lfb_size = GET_SIZE(get_back_tag(lfb_pos));
    dbgprint("new lfb at %u \n\r", lfb_pos);
  }
}

static INLINE_FUNCTION void _trunc_and_free(const TAG t, const TAG size) {
  /* Truncate a block and mark as freed */
  const TAG full_size = GET_SIZE(get_back_tag(t));
  const TAG free_pos =  (TAG)(t + size + (TAG)(2u * TAGSIZE));
  if ((full_size - size) >= (TAG)((2u*TAGSIZE)+1u)) {
    dbgprint("Truncating %u from %u to %u\n\r", t, full_size, size);
    write_tags(t, size);
    write_tags(free_pos, (full_size - size - (TAG)(2u * TAGSIZE)) | TAG_FREE_BIT);

    if ((TAG)(t + full_size + (TAG)(2u * TAGSIZE)) >= heap_top) {
      heap_top = free_pos;
      write_tags(heap_top, (TAG)MEMORY_SIZE - heap_top - (TAG)TAGSIZE);
    } else if (free_pos <= lfb_pos) {
      lfb_pos = free_pos;
      lfb_size = GET_SIZE(get_back_tag(lfb_pos));
      dbgprint("new lfb at %u \n\r", lfb_pos);
    } else {/* Could maybe concatenate blocks here? */}

  #ifdef USE_ERROR_DETECTION
    if(free_pos != heap_top) {write_crcs(free_pos, crc8(0xff, &_memory_space[free_pos], (full_size - size - (TAG)(2u * TAGSIZE))));}
    else {write_crcs(heap_top, crc8(0xff, &_memory_space[heap_top], (TAG)MEMORY_SIZE - heap_top - (TAG)TAGSIZE));}
  #endif

  } else {/* Not worth it */}
}

static TAG _padless_malloc(const size_t size) {
  /* Allocate a section of unpadded memory */
  const bool move_ht = (bool)(heap_top == lfb_pos); /* Will the top of the heap need to be moved? */
  TAG ret = 0;

  #ifdef USE_ERROR_DETECTION
    bool CRC_ERROR_DETECTION_IN_MALLOC;
  #endif

  #if (GUARDRAILS > 0)
    bool SPARE_HEAP_CHECK;
  #endif

  if (size != 0u) {
  #ifdef LAZY_INIT
    if (heap_top == 0u) {alloc_init();}
  #endif

    dbgprint("Allocating %u bytes\n\r", (int)size);
    if (((TAG)size < (TAG)(lfb_size - (2u * TAGSIZE))) && (lfb_size > (2u * TAGSIZE))) { /* Fit into lowest free block and split it */

      dbgprint("Size fits into lfb (size %u) with split at %u\n\r", (int)lfb_size, (int)lfb_pos);

    #ifdef USE_ERROR_DETECTION
      GUARDRAIL_ASSERT(crc_verify(lfb_pos), CRC_ERROR_DETECTION_IN_MALLOC);
    #endif

    #if (GUARDRAILS > 0)
      if (move_ht) {
        GUARDRAIL_ASSERT(((TAG)size < (TAG)(MEMORY_SIZE - heap_top - (TAG)(2u * TAGSIZE))), SPARE_HEAP_CHECK);
      }
    #endif

      ret = lfb_pos;
      lfb_pos = (TAG)(lfb_pos + (size) + (TAG)(2u * TAGSIZE));
      lfb_size = (TAG)(lfb_size - (size) - (TAG)(2u * TAGSIZE));

      write_tags(lfb_pos, lfb_size | TAG_FREE_BIT);
      write_tags(ret, size);

      #ifdef USE_ERROR_DETECTION
        write_crcs(lfb_pos, crc8(0xff, &_memory_space[lfb_pos], lfb_size));
      #endif

      if (move_ht) {
        heap_top = lfb_pos;
        lfb_size = (TAG)(MEMORY_SIZE - TAGSIZE - lfb_pos);
      }

      _lfb_search(ret, MAX_LFB_SEARCH_DISTANCE);
    } else if ((TAG)(size) <= lfb_size) { /* Fit into lowest free block and look for another */

      dbgprint("Size fits into lfb at %u\n\r", (int)lfb_pos);
    #ifdef USE_ERROR_DETECTION
      GUARDRAIL_ASSERT(crc_verify(lfb_pos), CRC_ERROR_DETECTION_IN_MALLOC);
    #endif
      ret = lfb_pos;
      write_tags(ret, lfb_size);
      _lfb_search(ret, MAX_LFB_SEARCH_DISTANCE);
    } else { /* Has to go at the top of the heap */

      dbgprint("Placing on heap top, %i\n\r", (int)heap_top);

    #if (GUARDRAILS > 0)
      GUARDRAIL_ASSERT(((TAG)(size) < (TAG)(MEMORY_SIZE - heap_top - (TAG)(2u * TAGSIZE))), SPARE_HEAP_CHECK);
    #endif

      ret = heap_top;
      write_tags(heap_top, (TAG)size);
      heap_top += (TAG)((TAG)(size) + (2u * TAGSIZE));
      write_tags(heap_top, ((TAG)MEMORY_SIZE - heap_top - (TAG)TAGSIZE) | TAG_FREE_BIT);

    #ifdef USE_ERROR_DETECTION
      write_crcs(heap_top, crc8(0xff, &_memory_space[heap_top], (TAG)MEMORY_SIZE - heap_top - (TAG)TAGSIZE));
    #endif

      if (move_ht) {
        lfb_pos = heap_top;
        lfb_size = (TAG)(MEMORY_SIZE - heap_top - TAGSIZE);
        dbgprint("new lfb at %u \n\r", lfb_pos);
      }
    }
  }
  return ret;
}

static void _padless_free(const TAG p) {

  TAG offs;
  unsigned int concatenations = 0;

#if (GUARDRAILS > 0)
  bool BLOCK_CORRUPTED_CHECK;
  bool ALREADY_FREE_CHECK;
#endif

#if (GUARDRAILS > 1)
  bool INVALID_PTR_CHECK;
#endif

#ifdef USE_ERROR_DETECTION
  bool CRC_ERROR_DETECTION_IN_FREE;
#endif

  dbgprint("Freeing %u\n\r", p);

  if (p < MEMORY_SIZE) {

    offs = (TAG)(p);

  #if (GUARDRAILS > 1)
    GUARDRAIL_ASSERT(((p >= TAGSIZE) && (p <= MEMORY_SIZE-TAGSIZE)), INVALID_PTR_CHECK);
  #endif

  #if (GUARDRAILS > 0)
    GUARDRAIL_ASSERT(((TAG)(offs + GET_SIZE(get_back_tag(offs))) <= MEMORY_SIZE - TAGSIZE && (get_back_tag(offs) == get_front_tag(offs))), BLOCK_CORRUPTED_CHECK); /* Verify tag integrity */
    GUARDRAIL_ASSERT(!TAG_FREE(get_back_tag(offs)), ALREADY_FREE_CHECK); /* Prevent double-free */
  #endif

    while ((concatenations < MAX_CONCAT_SEARCH_DISTANCE) && ((TAG)(offs + GET_SIZE(get_back_tag(offs))) <= (TAG)(MEMORY_SIZE - (2u*TAGSIZE)))) {
      if (TAG_FREE(get_next_tag(offs))) {
        dbgprint("Concatenating forward...\n\r");
        _concatenate_free_next(offs);
      } else {
        break;
      }
      concatenations++;
    }
    concatenations = 0;
    while ((concatenations < MAX_CONCAT_SEARCH_DISTANCE) && (offs >= (2u*TAGSIZE))) {
      if (TAG_FREE(get_prev_tag(offs))) {
        dbgprint("Concatenating backward...\n\r");
        offs = _concatenate_free_prev(offs);
      } else
      {
        break;
      }
      concatenations++;
    }
    dbgprint("Finished concatenations\n\r");
    write_tags(offs, get_back_tag(offs) | TAG_FREE_BIT);

    if (offs + GET_SIZE(get_back_tag(offs)) + (2u * TAGSIZE) >= heap_top) {
      dbgprint("Broke through to heap\n\r");
      heap_top = offs;
      write_tags(heap_top, ((TAG)MEMORY_SIZE - heap_top - (TAG)TAGSIZE));
    }

    _lfb_search_inclusive(offs, MAX_LFB_SEARCH_DISTANCE);
    if (offs <= lfb_pos) {
      lfb_pos = offs;
      lfb_size = GET_SIZE(get_back_tag(lfb_pos));
      dbgprint("new lfb at %u \n\r", lfb_pos);
    }

  #ifdef USE_ERROR_DETECTION
    GUARDRAIL_ASSERT(write_crcs_verify(offs, crc8(0xff, &_memory_space[offs], GET_SIZE(get_back_tag(offs)))), CRC_ERROR_DETECTION_IN_FREE);
  #endif
  }
  dbgprint("Freed %u\n\r", p);
  return;
}

static TAG _padless_realloc(const TAG p, const size_t size) {

  const TAG copy_size = GET_SIZE(get_back_tag(p));
  unsigned int concatenations = 0;
  bool move_lfb;
  TAG ret;
  TAG p0;
  TAG p1;
  TAG stop;

#if (GUARDRAILS > 0)
  bool BLOCK_CORRUPTED_CHECK;
#endif

#if (GUARDRAILS > 1)
  bool INVALID_PTR_CHECK;
#endif

  dbgprint("Reallocating %u to be %u\n\r", p, (int)size);

  if (size == 0u) {
    _padless_free(p);
    ret = 0;
  } else if (p < MEMORY_SIZE) {

  #if (GUARDRAILS > 1)
    GUARDRAIL_ASSERT(((p >= TAGSIZE) && (p <= MEMORY_SIZE-TAGSIZE)), INVALID_PTR_CHECK);
  #endif

  #if (GUARDRAILS > 0)
    GUARDRAIL_ASSERT((((TAG)(p + GET_SIZE(get_back_tag(p))) <= MEMORY_SIZE - TAGSIZE) && (get_back_tag(p) == get_front_tag(p))), BLOCK_CORRUPTED_CHECK);
  #endif

    if ((TAG)size <= GET_SIZE(get_back_tag(p))) {
      if ((TAG)size <= GET_SIZE(get_back_tag(p)) - (2u * TAGSIZE) - 1u) {_trunc_and_free(p, (TAG)size);}
      dbgprint("Truncate and free realloc\n\r");
      write_tags(p, GET_SIZE(get_back_tag(p)));
      ret = p;
    } else {
      /* Try to concatenate forward free blocks */
      while (TAG_FREE(get_next_tag(p)) && ((TAG)size >= GET_SIZE(get_back_tag(p))) && (concatenations < MAX_CONCAT_SEARCH_DISTANCE)) {
        dbgprint("Current size is %u\n\r", GET_SIZE(get_back_tag(p)));
        move_lfb = (bool)(p+GET_SIZE(get_back_tag(p)) + (2u*TAGSIZE) == lfb_pos);
        _concatenate_next(p);
        if(move_lfb) {_lfb_search(p, MAX_LFB_SEARCH_DISTANCE);}
        if ((TAG)(p + GET_SIZE(get_back_tag(p))) >= MEMORY_SIZE - TAGSIZE) {break;}
        concatenations++;
      }

      if ((TAG)size <= GET_SIZE(get_back_tag(p))) {
        dbgprint("Expanded to accomodate new size\n\r");
        if ((TAG)size < GET_SIZE(get_back_tag(p)) - (2u * TAGSIZE)) {_trunc_and_free(p, (TAG)size);}
        if(p+GET_SIZE(get_back_tag(p)) + (2u*TAGSIZE) > heap_top) {heap_top = p+GET_SIZE(get_back_tag(p)) + (2u*TAGSIZE);} /* Check if heap needs to be updated */
        write_tags(p, GET_SIZE(get_back_tag(p)));
        ret = p;
      } else {
        /* Have to move it somewhere else and copy */
        dbgprint("Full reallocate\n\r");
        ret = _padless_malloc(size);
        p0 = p;
        p1 = ret;
        stop = p0 + ((copy_size > size) ? (TAG)size : copy_size);
        while(p0 != stop) {_memory_space[p0] = _memory_space[p1]; p0++; p1++;}
        _padless_free(p);
      }
    }
  } else {ret = 0;}
  dbgprint("Returning %u\n\r", ret);
  return ret;
}

uint8_t* _malloc(const size_t size) {
  return ((size == 0u) ? NULL : (&_memory_space[_padless_malloc(size + ALLOC_LOW_PADDING + ALLOC_HIGH_PADDING)+ALLOC_LOW_PADDING]));
}

static void _free(const uint8_t p[]) {
  const TAG offset = ptr_to_offset(p); /* MISRA 2012 11.4 deviation happens */
  if(p != NULL) {_padless_free(offset-ALLOC_LOW_PADDING);}
}

uint8_t* _realloc(const uint8_t p[], const size_t size) {
  uint8_t* ret = NULL;
  if(size == 0u) {
    _free(p);
  }
  else {ret = &_memory_space[_padless_realloc(ptr_to_offset(p)-ALLOC_LOW_PADDING, size + ALLOC_LOW_PADDING + ALLOC_HIGH_PADDING) + ALLOC_LOW_PADDING];}  /* MISRA 2012 11.4 deviation happens */
  return ret;
}

uint8_t* _calloc(const size_t num, const size_t size) {
  uint8_t* ret = _malloc(size*num);
  uint8_t* p = ret;
  const uint8_t* stop = (const uint8_t*)&ret[size*num];
  while(p != stop) {*p = 0; p++;}
  return ret;
}
