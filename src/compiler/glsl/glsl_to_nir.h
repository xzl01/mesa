/*
 * Copyright © 2014 Intel Corporation
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice (including the next
 * paragraph) shall be included in all copies or substantial portions of the
 * Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 *
 * Authors:
 *    Connor Abbott (cwabbott0@gmail.com)
 *
 */

#ifndef GLSL_TO_NIR_H
#define GLSL_TO_NIR_H

#include "compiler/nir/nir.h"

#ifdef __cplusplus
extern "C" {
#endif

struct gl_context;
struct gl_constants;
struct gl_shader_program;

nir_shader *glsl_to_nir(const struct gl_constants *consts,
                        struct exec_list **ir, shader_info *si,
                        gl_shader_stage stage,
                        const nir_shader_compiler_options *options,
                        const uint8_t *src_blake3);

nir_shader *glsl_float64_funcs_to_nir(struct gl_context *ctx,
                                      const nir_shader_compiler_options *options);

#ifdef __cplusplus
}
#endif

#endif /* GLSL_TO_NIR_H */
