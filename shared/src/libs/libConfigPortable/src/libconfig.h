/* ----------------------------------------------------------------------------
   libconfig - A library for processing structured configuration files
   Copyright (C) 2005-2009  Mark A Lindner

   This file is part of libconfig.

   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public License
   as published by the Free Software Foundation; either version 2.1 of
   the License, or (at your option) any later version.

   This library is distributed in the hope that it will be useful, but
   WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Library General Public
   License along with this library; if not, see
   <http://www.gnu.org/licenses/>.
   ----------------------------------------------------------------------------
*/

#ifndef __libconfig_h
#define __libconfig_h

#define LIBCONFIG_STATIC
#include "fio.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#if defined(_WIN32) || defined(__WIN32__) || defined(WIN32)
#if defined(LIBCONFIG_STATIC)
#define LIBCONFIG_API
#elif defined(LIBCONFIG_EXPORTS)
#define LIBCONFIG_API __declspec(dllexport)
#else /* ! LIBCONFIG_EXPORTS */
#define LIBCONFIG_API __declspec(dllimport)
#endif /* LIBCONFIG_STATIC */
#else /* ! WIN32 */
#define LIBCONFIG_API
#endif /* WIN32 */

#include <stdio.h>

#define CONFIG_TYPE_NONE    0
#define CONFIG_TYPE_GROUP   1
#define CONFIG_TYPE_INT     2
#define CONFIG_TYPE_INT64   3
#define CONFIG_TYPE_FLOAT   4
#define CONFIG_TYPE_STRING  5
#define CONFIG_TYPE_BOOL    6
#define CONFIG_TYPE_ARRAY   7
#define CONFIG_TYPE_LIST    8

#define CONFIG_FORMAT_DEFAULT  0
#define CONFIG_FORMAT_HEX      1

#define CONFIG_OPTION_AUTOCONVERT 0x01

#define CONFIG_TRUE  (1)
#define CONFIG_FALSE (0)

typedef union config_value_t
{
  long ival;
  long long llval;
  double fval;
  char *sval;
  struct config_list_t *list;
} config_value_t;

typedef struct config_setting_t
{
  char *name;
  short type;
  short format;
  config_value_t value;
  struct config_setting_t *parent;
  struct config_t *config;
  void *hook;
  unsigned int line;
} config_setting_t;

typedef struct config_list_t
{
  unsigned int length;
  unsigned int capacity;
  config_setting_t **elements;
} config_list_t;

typedef struct config_t
{
  config_setting_t *root;
  void (*destructor)(void *);
  int flags;
  const char *error_text;
  int error_line;
} config_t;

extern LIBCONFIG_API int config_read(config_t *config, void* stream);
extern LIBCONFIG_API void config_write(const config_t *config, void* stream);

extern LIBCONFIG_API void config_set_auto_convert(config_t *config, int flag);
extern LIBCONFIG_API int config_get_auto_convert(const config_t *config);

extern LIBCONFIG_API int config_read_file(config_t *config,
                                          const char *filename);
extern LIBCONFIG_API int config_write_file(config_t *config,
                                           const char *filename);

extern LIBCONFIG_API void config_set_destructor(config_t *config,
                                                void (*destructor)(void *));

extern LIBCONFIG_API void config_init(config_t *config);
extern LIBCONFIG_API void config_destroy(config_t *config);

extern LIBCONFIG_API long config_setting_get_int(
  const config_setting_t *setting);
extern LIBCONFIG_API long long config_setting_get_int64(
  const config_setting_t *setting);
extern LIBCONFIG_API double config_setting_get_float(
  const config_setting_t *setting);
extern LIBCONFIG_API int config_setting_get_bool(
  const config_setting_t *setting);
extern LIBCONFIG_API const char *config_setting_get_string(
  const config_setting_t *setting);

extern LIBCONFIG_API int config_setting_lookup_int(
  const config_setting_t *setting, const char *name, long *value);
extern LIBCONFIG_API int config_setting_lookup_int64(
  const config_setting_t *setting, const char *name, long long *value);
extern LIBCONFIG_API int config_setting_lookup_float(
  const config_setting_t *setting, const char *name, double *value);
extern LIBCONFIG_API int config_setting_lookup_bool(
  const config_setting_t *setting, const char *name, int *value);
extern LIBCONFIG_API int config_setting_lookup_string(
  const config_setting_t *setting, const char *name, const char **value);

extern LIBCONFIG_API int config_setting_set_int(config_setting_t *setting,
                                                long value);
extern LIBCONFIG_API int config_setting_set_int64(config_setting_t *setting,
                                                  long long value);
extern LIBCONFIG_API int config_setting_set_float(config_setting_t *setting,
                                                  double value);
extern LIBCONFIG_API int config_setting_set_bool(config_setting_t *setting,
                                                 int value);
extern LIBCONFIG_API int config_setting_set_string(config_setting_t *setting,
                                                   const char *value);

extern LIBCONFIG_API int config_setting_set_format(config_setting_t *setting,
                                                   short format);
extern LIBCONFIG_API short config_setting_get_format(config_setting_t *setting);

extern LIBCONFIG_API long config_setting_get_int_elem(
  const config_setting_t *setting, int idx);
extern LIBCONFIG_API long long config_setting_get_int64_elem(
  const config_setting_t *setting, int idx);
extern LIBCONFIG_API double config_setting_get_float_elem(
  const config_setting_t *setting, int idx);
extern LIBCONFIG_API int config_setting_get_bool_elem(
  const config_setting_t *setting, int idx);
extern LIBCONFIG_API const char *config_setting_get_string_elem(
  const config_setting_t *setting, int idx);

extern LIBCONFIG_API config_setting_t *config_setting_set_int_elem(
  config_setting_t *setting, int idx, long value);
extern LIBCONFIG_API config_setting_t *config_setting_set_int64_elem(
  config_setting_t *setting, int idx, long long value);
extern LIBCONFIG_API config_setting_t *config_setting_set_float_elem(
  config_setting_t *setting, int idx, double value);
extern LIBCONFIG_API config_setting_t *config_setting_set_bool_elem(
  config_setting_t *setting, int idx, int value);
extern LIBCONFIG_API config_setting_t *config_setting_set_string_elem(
  config_setting_t *setting, int idx, const char *value);

#define /* int */ config_setting_type(/* const config_setting_t * */ S) \
  ((S)->type)

#define /* int */ config_setting_is_group(/* const config_setting_t * */ S) \
  ((S)->type == CONFIG_TYPE_GROUP)
#define /* int */ config_setting_is_array(/* const config_setting_t * */ S) \
  ((S)->type == CONFIG_TYPE_ARRAY)
#define /* int */ config_setting_is_list(/* const config_setting_t * */ S) \
  ((S)->type == CONFIG_TYPE_LIST)

#define /* int */ config_setting_is_aggregate( \
  /* const config_setting_t * */ S)                                     \
  (((S)->type == CONFIG_TYPE_GROUP) || ((S)->type == CONFIG_TYPE_LIST)  \
   || ((S)->type == CONFIG_TYPE_ARRAY))
  
#define /* int */ config_setting_is_number(/* const config_setting_t * */ S) \
  (((S)->type == CONFIG_TYPE_INT)                                       \
   || ((S)->type == CONFIG_TYPE_INT64)                                  \
   || ((S)->type == CONFIG_TYPE_FLOAT))
  
#define /* int */ config_setting_is_scalar(/* const config_setting_t * */ S) \
  (((S)->type == CONFIG_TYPE_BOOL) || ((S)->type == CONFIG_TYPE_STRING) \
   || config_setting_is_number(S))

#define /* const char * */ config_setting_name( \
  /* const config_setting_t * */ S)             \
  ((S)->name)

#define /* config_setting_t * */ config_setting_parent( \
  /* const config_setting_t * */ S)                     \
  ((S)->parent)

#define /* int */ config_setting_is_root(       \
  /* const config_setting_t * */ S)             \
  ((S)->parent ? CONFIG_FALSE : CONFIG_TRUE)

extern LIBCONFIG_API int config_setting_index(const config_setting_t *setting);

extern LIBCONFIG_API int config_setting_length(
  const config_setting_t *setting);
extern LIBCONFIG_API config_setting_t *config_setting_get_elem(
  const config_setting_t *setting, unsigned int idx);

extern LIBCONFIG_API config_setting_t *config_setting_get_member(
  const config_setting_t *setting, const char *name);

extern LIBCONFIG_API config_setting_t *config_setting_add(
  config_setting_t *parent, const char *name, int type);
extern LIBCONFIG_API int config_setting_remove(config_setting_t *parent,
                                               const char *name);
extern LIBCONFIG_API int config_setting_remove_elem(config_setting_t *parent,
                                                    unsigned int idx);
extern LIBCONFIG_API void config_setting_set_hook(config_setting_t *setting,
                                                  void *hook);

#define config_setting_get_hook(S) ((S)->hook)

extern LIBCONFIG_API config_setting_t *config_lookup(const config_t *config,
                                                     const char *path);

extern LIBCONFIG_API int config_lookup_int(const config_t *config,
                                           const char *path, long *value);
extern LIBCONFIG_API int config_lookup_int64(const config_t *config,
                                             const char *path,
                                             long long *value);
extern LIBCONFIG_API int config_lookup_float(const config_t *config,
                                             const char *path, double *value);
extern LIBCONFIG_API int config_lookup_bool(const config_t *config,
                                            const char *path, int *value);
extern LIBCONFIG_API int config_lookup_string(const config_t *config,
                                              const char *path,
                                              const char **value);

#define /* config_setting_t * */ config_root_setting( \
  /* const config_t * */ C)                           \
  ((C)->root)

#define /* unsigned short */ config_setting_source_line( \
  /* const config_t */ C)                                \
  ((C)->line)

#define /* const char * */ config_error_text(/* const config_t * */ C)  \
  ((C)->error_text)

#define /* int */ config_error_line(/* const config_t * */ C)   \
  ((C)->error_line)

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __libconfig_h */
