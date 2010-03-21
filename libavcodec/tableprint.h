/*
 * Generate a file for hardcoded tables
 *
 * Copyright (c) 2009 Reimar Döffinger <Reimar.Doeffinger@gmx.de>
 *
 * This file is part of FFmpeg.
 *
 * FFmpeg is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * FFmpeg is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with FFmpeg; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 */

#ifndef AVCODEC_TABLEPRINT_H
#define AVCODEC_TABLEPRINT_H

#include <stdint.h>
#include <stdio.h>

#define WRITE_1D_FUNC_ARGV(name, type, linebrk, fmtstr, ...)\
void write_##name##_array(const void *arg, int len, int dummy)\
{\
    const type *data = arg;\
    int i;\
    printf("   ");\
    for (i = 0; i < len - 1; i++) {\
       printf(" "fmtstr",", __VA_ARGS__);\
       if ((i & linebrk) == linebrk) printf("\n   ");\
    }\
    printf(" "fmtstr"\n", __VA_ARGS__);\
}

#define WRITE_1D_FUNC(name, type, fmtstr, linebrk)\
    WRITE_1D_FUNC_ARGV(name, type, linebrk, fmtstr, data[i])

#define WRITE_2D_FUNC(name, type)\
void write_##name##_2d_array(const void *arg, int len, int len2)\
{\
    const type *data = arg;\
    int i;\
    printf("    {\n");\
    for (i = 0; i < len; i++) {\
        write_##name##_array(data + i * len2, len2, 0);\
        printf(i == len - 1 ? "    }\n" : "    }, {\n");\
    }\
}

/**
 * \defgroup printfuncs Predefined functions for printing tables
 *
 * \{
 */
void write_int8_array     (const void *, int, int);
void write_uint8_array    (const void *, int, int);
void write_uint16_array   (const void *, int, int);
void write_uint32_array   (const void *, int, int);
void write_float_array    (const void *, int, int);
void write_int8_2d_array  (const void *, int, int);
void write_uint8_2d_array (const void *, int, int);
void write_uint32_2d_array(const void *, int, int);
/** \} */ // end of printfuncs group

struct tabledef {
    /** String that declares the array. Adding " = { ..." after it should
     * make a valid initializer, adding "extern" before and ";" if possible
     * should make a valid extern declaration. */
    const char *declaration;
    /** Function used to print the table data (i.e. the part in {}).
     * Should be one of the predefined write_*_array functions. */
    void (*printfunc)(const void *, int, int);
    /** Pointer passed to the printfunc, usually a pointer to the start
     * of the array to be printed. */
    const void *data;
    int size;   ///< size of the first dimension of the array
    int size2;  ///< size of the second dimension of the array if any
};

/** Initializes all the tables described in the tables array */
void tableinit(void);
/** Describes the tables that should be printed */
extern const struct tabledef tables[];

#endif /* AVCODEC_TABLEPRINT_H */
