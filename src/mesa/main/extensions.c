/*
 * Mesa 3-D graphics library
 *
 * Copyright (C) 1999-2008  Brian Paul   All Rights Reserved.
 * Copyright (C) 2009  VMware, Inc.  All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR
 * OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 */


/**
 * \file
 * \brief Extension handling
 */

#include "util/os_misc.h"

#include "glheader.h"

#include "context.h"
#include "extensions.h"
#include "macros.h"
#include "mtypes.h"

struct gl_extensions _mesa_extension_override_enables;
struct gl_extensions _mesa_extension_override_disables;

#define MAX_UNRECOGNIZED_EXTENSIONS 16
static struct {
   char *env;
   const char *names[MAX_UNRECOGNIZED_EXTENSIONS];
} unrecognized_extensions;

/**
 * Given a member \c x of struct gl_extensions, return offset of
 * \c x in bytes.
 */
#define o(x) offsetof(struct gl_extensions, x)

static int
extension_name_compare(const void *name, const void *elem)
{
   const struct mesa_extension *entry = elem;
   return strcmp(name, entry->name);
}

/**
 * Given an extension name, lookup up the corresponding member of struct
 * gl_extensions and return that member's index.  If the name is
 * not found in the \c _mesa_extension_table, return -1.
 *
 * \param name Name of extension.
 * \return Index of member in struct gl_extensions.
 */
static int
name_to_index(const char* name)
{
   const struct mesa_extension *entry;

   if (!name)
      return -1;

   entry = bsearch(name,
                   _mesa_extension_table, MESA_EXTENSION_COUNT,
                   sizeof(_mesa_extension_table[0]),
                   extension_name_compare);

   if (entry)
      return entry - _mesa_extension_table;

   return -1;
}

/**
 * Overrides extensions in \c ctx based on the values in
 * _mesa_extension_override_enables and _mesa_extension_override_disables.
 */
void
_mesa_override_extensions(struct gl_context *ctx)
{
   unsigned i;
   const GLboolean *enables =
      (GLboolean*) &_mesa_extension_override_enables;
   const GLboolean *disables =
      (GLboolean*) &_mesa_extension_override_disables;
   GLboolean *ctx_ext = (GLboolean*)&ctx->Extensions;

   for (i = 0; i < MESA_EXTENSION_COUNT; ++i) {
      size_t offset = _mesa_extension_table[i].offset;

      assert(!enables[offset] || !disables[offset]);
      if (enables[offset]) {
         ctx_ext[offset] = 1;
      } else if (disables[offset]) {
         ctx_ext[offset] = 0;
      }
   }
}


/**
 * Enable all extensions suitable for a software-only renderer.
 * This is a convenience function used by the mesa/swrast drivers.
 */
void
_mesa_enable_sw_extensions(struct gl_context *ctx)
{
   ctx->Extensions.ARB_depth_clamp = GL_TRUE;
   ctx->Extensions.ARB_depth_texture = GL_TRUE;
   ctx->Extensions.ARB_draw_elements_base_vertex = GL_TRUE;
   ctx->Extensions.ARB_draw_instanced = GL_TRUE;
   ctx->Extensions.ARB_explicit_attrib_location = GL_TRUE;
   ctx->Extensions.ARB_fragment_coord_conventions = GL_TRUE;
   ctx->Extensions.ARB_fragment_program = GL_TRUE;
   ctx->Extensions.ARB_fragment_program_shadow = GL_TRUE;
   ctx->Extensions.ARB_fragment_shader = GL_TRUE;
   ctx->Extensions.ARB_framebuffer_object = GL_TRUE;
   ctx->Extensions.ARB_half_float_vertex = GL_TRUE;
   ctx->Extensions.ARB_map_buffer_range = GL_TRUE;
   ctx->Extensions.ARB_occlusion_query = GL_TRUE;
   ctx->Extensions.ARB_occlusion_query2 = GL_TRUE;
   ctx->Extensions.ARB_point_sprite = GL_TRUE;
   ctx->Extensions.ARB_shadow = GL_TRUE;
   ctx->Extensions.ARB_texture_border_clamp = GL_TRUE;
   ctx->Extensions.ARB_texture_compression_bptc = GL_TRUE;
   ctx->Extensions.ARB_texture_cube_map = GL_TRUE;
   ctx->Extensions.ARB_texture_env_combine = GL_TRUE;
   ctx->Extensions.ARB_texture_env_crossbar = GL_TRUE;
   ctx->Extensions.ARB_texture_env_dot3 = GL_TRUE;
   ctx->Extensions.ARB_texture_filter_anisotropic = GL_TRUE;
   ctx->Extensions.ARB_texture_float = GL_TRUE;
   ctx->Extensions.ARB_texture_mirror_clamp_to_edge = GL_TRUE;
   ctx->Extensions.ARB_texture_non_power_of_two = GL_TRUE;
   ctx->Extensions.ARB_texture_rg = GL_TRUE;
   ctx->Extensions.ARB_texture_compression_rgtc = GL_TRUE;
   ctx->Extensions.ARB_vertex_program = GL_TRUE;
   ctx->Extensions.ARB_vertex_shader = GL_TRUE;
   ctx->Extensions.ARB_sync = GL_TRUE;
   ctx->Extensions.APPLE_object_purgeable = GL_TRUE;
   ctx->Extensions.ATI_fragment_shader = GL_TRUE;
   ctx->Extensions.ATI_texture_compression_3dc = GL_TRUE;
   ctx->Extensions.ATI_texture_env_combine3 = GL_TRUE;
   ctx->Extensions.ATI_texture_mirror_once = GL_TRUE;
   ctx->Extensions.EXT_blend_color = GL_TRUE;
   ctx->Extensions.EXT_blend_equation_separate = GL_TRUE;
   ctx->Extensions.EXT_blend_func_separate = GL_TRUE;
   ctx->Extensions.EXT_blend_minmax = GL_TRUE;
   ctx->Extensions.EXT_depth_bounds_test = GL_TRUE;
   ctx->Extensions.EXT_draw_buffers2 = GL_TRUE;
   ctx->Extensions.EXT_pixel_buffer_object = GL_TRUE;
   ctx->Extensions.EXT_point_parameters = GL_TRUE;
   ctx->Extensions.EXT_provoking_vertex = GL_TRUE;
   ctx->Extensions.EXT_stencil_two_side = GL_TRUE;
   ctx->Extensions.EXT_texture_array = GL_TRUE;
   ctx->Extensions.EXT_texture_compression_latc = GL_TRUE;
   ctx->Extensions.EXT_texture_env_dot3 = GL_TRUE;
   ctx->Extensions.EXT_texture_filter_anisotropic = GL_TRUE;
   ctx->Extensions.EXT_texture_mirror_clamp = GL_TRUE;
   ctx->Extensions.EXT_texture_shared_exponent = GL_TRUE;
   ctx->Extensions.EXT_texture_sRGB = GL_TRUE;
   ctx->Extensions.EXT_texture_sRGB_decode = GL_TRUE;
   ctx->Extensions.EXT_texture_swizzle = GL_TRUE;
   /*ctx->Extensions.EXT_transform_feedback = GL_TRUE;*/
   ctx->Extensions.EXT_vertex_array_bgra = GL_TRUE;
   ctx->Extensions.MESA_ycbcr_texture = GL_TRUE;
   ctx->Extensions.NV_conditional_render = GL_TRUE;
   ctx->Extensions.NV_texture_env_combine4 = GL_TRUE;
   ctx->Extensions.NV_texture_rectangle = GL_TRUE;
   ctx->Extensions.EXT_gpu_program_parameters = GL_TRUE;
   ctx->Extensions.OES_standard_derivatives = GL_TRUE;
   ctx->Extensions.TDFX_texture_compression_FXT1 = GL_TRUE;
   ctx->Extensions.ANGLE_texture_compression_dxt = GL_TRUE;
   ctx->Extensions.EXT_texture_compression_s3tc = GL_TRUE;
}

/**
 * Either enable or disable the named extension.
 * \return offset of extensions withint `ext' or 0 if extension is not known
 */
static size_t
set_extension(struct gl_extensions *ext, int i, GLboolean state)
{
   size_t offset;

   offset = i < 0 ? 0 : _mesa_extension_table[i].offset;
   if (offset != 0 && (offset != o(dummy_true) || state != GL_FALSE)) {
      ((GLboolean *) ext)[offset] = state;
   }

   return offset;
}


/**
 * \brief Free string pointed by unrecognized_extensions
 *
 * This string is allocated early during the first context creation by
 * _mesa_one_time_init_extension_overrides.
 */
static void
free_unknown_extensions_strings(void)
{
   free(unrecognized_extensions.env);
   for (int i = 0; i < MAX_UNRECOGNIZED_EXTENSIONS; ++i)
      unrecognized_extensions.names[i] = NULL;
}


/**
 * \brief Initialize extension override tables based on \c MESA_EXTENSION_OVERRIDE
 *
 * This should be called one time early during first context initialization.

 * \c MESA_EXTENSION_OVERRIDE is a space-separated list of extensions to
 * enable or disable. The list is processed thus:
 *    - Enable recognized extension names that are prefixed with '+'.
 *    - Disable recognized extension names that are prefixed with '-'.
 *    - Enable recognized extension names that are not prefixed.
 *    - Collect unrecognized extension names in a new string.
 */
void
_mesa_one_time_init_extension_overrides(void)
{
   const char *env_const = os_get_option("MESA_EXTENSION_OVERRIDE");
   char *env;
   char *ext;
   size_t offset;
   unsigned unknown_ext = 0;

   memset(&_mesa_extension_override_enables, 0, sizeof(struct gl_extensions));
   memset(&_mesa_extension_override_disables, 0, sizeof(struct gl_extensions));

   if (env_const == NULL) {
      return;
   }

   /* Copy env_const because strtok() is destructive. */
   env = strdup(env_const);

   if (env == NULL)
      return;

   for (ext = strtok(env, " "); ext != NULL; ext = strtok(NULL, " ")) {
      int enable;
      int i;
      bool recognized;
      switch (ext[0]) {
      case '+':
         enable = 1;
         ++ext;
         break;
      case '-':
         enable = 0;
         ++ext;
         break;
      default:
         enable = 1;
         break;
      }

      i = name_to_index(ext);
      offset = set_extension(&_mesa_extension_override_enables, i, enable);
      offset = set_extension(&_mesa_extension_override_disables, i, !enable);
      if (offset != 0)
         recognized = true;
      else
         recognized = false;

      if (!recognized && enable) {
         if (unknown_ext >= MAX_UNRECOGNIZED_EXTENSIONS) {
            static bool warned;

            if (!warned) {
               warned = true;
               _mesa_problem(NULL, "Trying to enable too many unknown extension. "
                                   "Only the first %d will be honoured",
                                   MAX_UNRECOGNIZED_EXTENSIONS);
            }
         } else {
            unrecognized_extensions.names[unknown_ext] = ext;
            unknown_ext++;
            _mesa_problem(NULL, "Trying to enable unknown extension: %s", ext);
         }
      }
   }

   if (!unknown_ext) {
      free(env);
   } else {
      unrecognized_extensions.env = env;
      atexit(free_unknown_extensions_strings);
   }
}


/**
 * \brief Initialize extension tables and enable default extensions.
 *
 * This should be called during context initialization.
 * Note: Sets gl_extensions.dummy_true to true.
 */
void
_mesa_init_extensions(struct gl_extensions *extensions)
{
   GLboolean *base = (GLboolean *) extensions;
   GLboolean *sentinel = base + o(extension_sentinel);
   GLboolean *i;

   /* First, turn all extensions off. */
   for (i = base; i != sentinel; ++i)
      *i = GL_FALSE;

   /* Then, selectively turn default extensions on. */
   extensions->dummy_true = GL_TRUE;
}


typedef unsigned short extension_index;


/**
 * Given an extension enum, return whether or not the extension is supported
 * dependent on the following factors:
 * There's driver support and the OpenGL/ES version is at least that
 * specified in the _mesa_extension_table.
 */
static inline bool
_mesa_extension_supported(const struct gl_context *ctx, extension_index i)
{
   const bool *base = (bool *) &ctx->Extensions;
   const struct mesa_extension *ext = _mesa_extension_table + i;

   return (ctx->Version >= ext->version[ctx->API]) && base[ext->offset];
}

/**
 * Compare two entries of the extensions table.  Sorts first by year,
 * then by name.
 *
 * Arguments are indices into _mesa_extension_table.
 */
static int
extension_compare(const void *p1, const void *p2)
{
   extension_index i1 = * (const extension_index *) p1;
   extension_index i2 = * (const extension_index *) p2;
   const struct mesa_extension *e1 = &_mesa_extension_table[i1];
   const struct mesa_extension *e2 = &_mesa_extension_table[i2];
   int res;

   res = (int)e1->year - (int)e2->year;

   if (res == 0) {
      res = strcmp(e1->name, e2->name);
   }

   return res;
}


/**
 * Construct the GL_EXTENSIONS string.  Called the first time that
 * glGetString(GL_EXTENSIONS) is called.
 */
GLubyte*
_mesa_make_extension_string(struct gl_context *ctx)
{
   /* The extension string. */
   char *exts = 0;
   /* Length of extension string. */
   size_t length = 0;
   /* Number of extensions */
   unsigned count;
   /* Indices of the extensions sorted by year */
   extension_index extension_indices[MESA_EXTENSION_COUNT];
   unsigned k;
   unsigned j;
   unsigned maxYear = ~0;

   /* Check if the MESA_EXTENSION_MAX_YEAR env var is set */
   {
      const char *env = getenv("MESA_EXTENSION_MAX_YEAR");
      if (env) {
         maxYear = atoi(env);
         _mesa_debug(ctx, "Note: limiting GL extensions to %u or earlier\n",
                     maxYear);
      }
   }

   /* Compute length of the extension string. */
   count = 0;
   for (k = 0; k < MESA_EXTENSION_COUNT; ++k) {
      const struct mesa_extension *i = _mesa_extension_table + k;

      if (i->year <= maxYear &&
          _mesa_extension_supported(ctx, k)) {
	 length += strlen(i->name) + 1; /* +1 for space */
	 ++count;
      }
   }
   for (k = 0; k < MAX_UNRECOGNIZED_EXTENSIONS; k++)
      if (unrecognized_extensions.names[k])
         length += 1 + strlen(unrecognized_extensions.names[k]); /* +1 for space */

   exts = calloc(ALIGN(length + 1, 4), sizeof(char));
   if (exts == NULL) {
      return NULL;
   }

   /* Sort extensions in chronological order because idTech 2/3 games
    * (e.g., Quake3 demo) store the extension list in a fixed size buffer.
    * Some cases truncate, while others overflow the buffer. Resulting in
    * misrendering and crashes, respectively.
    * Address the former here, while the latter will be addressed by setting
    * the MESA_EXTENSION_MAX_YEAR environment variable.
    */
   j = 0;
   for (k = 0; k < MESA_EXTENSION_COUNT; ++k) {
      if (_mesa_extension_table[k].year <= maxYear &&
         _mesa_extension_supported(ctx, k)) {
         extension_indices[j++] = k;
      }
   }
   assert(j == count);
   qsort(extension_indices, count,
         sizeof *extension_indices, extension_compare);

   /* Build the extension string.*/
   for (j = 0; j < count; ++j) {
      const struct mesa_extension *i = &_mesa_extension_table[extension_indices[j]];
      assert(_mesa_extension_supported(ctx, extension_indices[j]));
      strcat(exts, i->name);
      strcat(exts, " ");
   }
   for (j = 0; j < MAX_UNRECOGNIZED_EXTENSIONS; j++) {
      if (unrecognized_extensions.names[j]) {
         strcat(exts, unrecognized_extensions.names[j]);
         strcat(exts, " ");
      }
   }

   return (GLubyte *) exts;
}

/**
 * Return number of enabled extensions.
 */
GLuint
_mesa_get_extension_count(struct gl_context *ctx)
{
   unsigned k;

   /* only count once */
   if (ctx->Extensions.Count != 0)
      return ctx->Extensions.Count;

   for (k = 0; k < MESA_EXTENSION_COUNT; ++k) {
      if (_mesa_extension_supported(ctx, k))
	 ctx->Extensions.Count++;
   }

   for (k = 0; k < MAX_UNRECOGNIZED_EXTENSIONS; ++k) {
      if (unrecognized_extensions.names[k])
         ctx->Extensions.Count++;
   }
   return ctx->Extensions.Count;
}

/**
 * Return name of i-th enabled extension
 */
const GLubyte *
_mesa_get_enabled_extension(struct gl_context *ctx, GLuint index)
{
   size_t n = 0;
   unsigned i;

   for (i = 0; i < MESA_EXTENSION_COUNT; ++i) {
      if (_mesa_extension_supported(ctx, i)) {
         if (n == index)
            return (const GLubyte*) _mesa_extension_table[i].name;
         else
            ++n;
      }
   }

   for (i = 0; i < MAX_UNRECOGNIZED_EXTENSIONS; ++i) {
      if (unrecognized_extensions.names[i]) {
         if (n == index)
            return (const GLubyte*) unrecognized_extensions.names[i];
         else
            ++n;
      }
   }
   return NULL;
}
