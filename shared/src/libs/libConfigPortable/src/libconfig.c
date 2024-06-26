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

#ifdef HAVE_CONFIG_H
#include "ac_config.h"
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "libconfig.h"
#include "grammar.h"
#include "scanner.h"
#include "private.h"
#include "wincompat.h"
#include "fio.h"

#if 1 // def _MBCS
#include <locale.h>
#endif

#ifdef HAVE_XLOCALE_H
#include <xlocale.h>
#endif

#include <stdlib.h>
#include <ctype.h>

#define PATH_TOKENS ":./"
#define CHUNK_SIZE 10
#define FLOAT_PRECISION 10

#define _new(T) (T *)calloc(sizeof(T), 1) /* zeroed */
#define _delete(P) free((void *)(P))

#if 0 // ndef _MBCS
char *strdup(const char *s1);
#endif



/* ------------------------------------------------------------------------- */

static const char *___io_error = "file I/O error";

static void __config_list_destroy(config_list_t *list);
static void __config_write_setting(const config_setting_t *setting,
                                   FIOHANDLER stream, int depth);

extern int libconfig_yyparse(void *scanner, struct parse_context *ctx);

/* ------------------------------------------------------------------------- */

static void __config_locale_override()
{
#if (defined(WIN32) || defined(_WIN32) || defined(__WIN32__)) \
  && ! defined(__MINGW32__)

  _configthreadlocale(_ENABLE_PER_THREAD_LOCALE);
  setlocale(LC_NUMERIC, "C");

#elif defined(__APPLE__)

  locale_t loc = newlocale(LC_NUMERIC_MASK, "C", NULL);
  uselocale(loc);

#elif ((defined HAVE_NEWLOCALE) && (defined HAVE_USELOCALE))

  locale_t loc = newlocale(LC_NUMERIC, "C", NULL);
  uselocale(loc);

#else

#warning "No way to modify calling thread's locale!"

#endif
}

/* ------------------------------------------------------------------------- */

static void __config_locale_restore()
{
#if (defined(WIN32) || defined(_WIN32) || defined(__WIN32__)) \
  && ! defined(__MINGW32__)

    _configthreadlocale(_DISABLE_PER_THREAD_LOCALE);

#elif ((defined HAVE_USELOCALE) && (defined HAVE_FREELOCALE))

  locale_t loc = uselocale(LC_GLOBAL_LOCALE);
  freelocale(loc);

#else

#warning "No way to modify calling thread's locale!"

#endif
}

/* ------------------------------------------------------------------------- */

static int __config_name_compare(const char *a, const char *b)
{
  const char *p, *q;

  for(p = a, q = b; ; p++, q++)
  {
    int pd = ((! *p) || strchr(PATH_TOKENS, *p));
    int qd = ((! *q) || strchr(PATH_TOKENS, *q));

    if(pd && qd)
      break;
    else if(pd)
      return(-1);
    else if(qd)
      return(1);
    else if(*p < *q)
      return(-1);
    else if(*p > *q)
      return(1);
  }

  return(0);
}

/* ------------------------------------------------------------------------- */

static void __config_write_value(const config_value_t *value, int type,
                                 int format, int depth, FIOHANDLER stream)
{
	#ifndef READONLY
  char fbuf[64];

  switch(type)
  {
    /* boolean */
    case CONFIG_TYPE_BOOL:
      fputs(value->ival ? "true" : "false", stream);
      break;

    /* int */
    case CONFIG_TYPE_INT:
      switch(format)
      {
        case CONFIG_FORMAT_HEX:
          fprintf(stream, "0x%lX", value->ival);
          break;

        case CONFIG_FORMAT_DEFAULT:
        default:
          fprintf(stream, "%ld", value->ival);
          break;
      }
      break;

    /* 64-bit int */
    case CONFIG_TYPE_INT64:
      switch(format)
      {
        case CONFIG_FORMAT_HEX:
          fprintf(stream, "0x" INT64_HEX_FMT "L", value->llval);
          break;

        case CONFIG_FORMAT_DEFAULT:
        default:
          fprintf(stream, INT64_FMT "L", value->llval);
          break;
      }
      break;

    /* float */
    case CONFIG_TYPE_FLOAT:
    {
      char *q;

      snprintf(fbuf, sizeof(fbuf) - 3, "%.*g", FLOAT_PRECISION, value->fval);

      /* check for exponent */

      q = strchr(fbuf, 'e');
      if(! q)
      {
        /* no exponent */

        if(! strchr(fbuf, '.')) /* no decimal point */
          strcat(fbuf, ".0");
        else
        {
          /* has decimal point */

          char *p;

          for(p = fbuf + strlen(fbuf) - 1; p > fbuf; --p)
          {
            if(*p != '0')
            {
              *(++p) = '\0';
              break;
            }
          }
        }
      }

      fputs(fbuf, stream);
      break;
    }

    /* string */
    case CONFIG_TYPE_STRING:
    {
      char *p;

      fputc('\"', stream);

      if(value->sval)
      {
        for(p = value->sval; *p; p++)
        {
          switch(*p)
          {
            case '\"':
            case '\\':
              fputc('\\', stream);
              fputc(*p, stream);
              break;

            case '\n':
              fputs("\\n", stream);
              break;

            case '\r':
              fputs("\\r", stream);
              break;

            case '\f':
              fputs("\\f", stream);
              break;

            case '\t':
              fputs("\\t", stream);
              break;

            default:
              fputc(*p, stream);
          }
        }
      }
      fputc('\"', stream);
      break;
    }

    /* list */
    case CONFIG_TYPE_LIST:
    {
      config_list_t *list = value->list;

      fprintf(stream, "( ");

      if(list)
      {
        int len = list->length;
        config_setting_t **s;

        for(s = list->elements; len--; s++)
        {
          __config_write_value(&((*s)->value), (*s)->type, (*s)->format,
                               depth + 1, stream);

          if(len)
            fputc(',', stream);

          fputc(' ', stream);
        }
      }

      fputc(')', stream);
      break;
    }

    /* array */
    case CONFIG_TYPE_ARRAY:
    {
      config_list_t *list = value->list;

      fprintf(stream, "[ ");

      if(list)
      {
        int len = list->length;
        config_setting_t **s;

        for(s = list->elements; len--; s++)
        {
          __config_write_value(&((*s)->value), (*s)->type, (*s)->format,
                               depth + 1, stream);

          if(len)
            fputc(',', stream);

          fputc(' ', stream);
        }
      }

      fputc(']', stream);
      break;
    }

    /* group */
    case CONFIG_TYPE_GROUP:
    {
      config_list_t *list = value->list;

      if(depth > 0)
      {
#ifdef K_AND_R_STYLE /* Horrendous, but many people like it. */
        fputc(' ', stream);
#else
        fputc('\n', stream);

        if(depth > 1)
          fprintf(stream, "%*s", (depth - 1) * 2, " ");
#endif
        fprintf(stream, "{\n");
      }

      if(list)
      {
        int len = list->length;
        config_setting_t **s;

        for(s = list->elements; len--; s++)
          __config_write_setting(*s, stream, depth + 1);
      }

      if(depth > 1)
        fprintf(stream, "%*s", (depth - 1) * 2, " ");

      if(depth > 0)
        fputc('}', stream);

      break;
    }

    default:
      /* this shouldn't happen, but handle it gracefully... */
      fputs("???", stream);
      break;
  }
  #endif
}

/* ------------------------------------------------------------------------- */

static void __config_list_add(config_list_t *list, config_setting_t *setting)
{
  if(list->length == list->capacity)
  {
    list->capacity += CHUNK_SIZE;
    list->elements = (config_setting_t **)realloc(
      list->elements, list->capacity * sizeof(config_setting_t *));
  }

  list->elements[list->length] = setting;
  list->length++;
}

/* ------------------------------------------------------------------------- */

static config_setting_t *__config_list_search(config_list_t *list,
                                              const char *name,
                                              unsigned int *idx)
{
  config_setting_t **found = NULL;
  unsigned int i;

  if(! list)
    return(NULL);

  for(i = 0, found = list->elements; i < list->length; i++, found++)
  {
    if(! (*found)->name)
      continue;

    if(! __config_name_compare(name, (*found)->name))
    {
      if(idx)
        *idx = i;

      return(*found);
    }
  }

  return(NULL);
}

/* ------------------------------------------------------------------------- */

static config_setting_t *__config_list_remove(config_list_t *list, int idx)
{
  config_setting_t *removed = *(list->elements + idx);
  int offset = (idx * sizeof(config_setting_t *));
  int len = list->length - 1 - idx;
  char *base = (char *)list->elements + offset;

  memmove(base, base + sizeof(config_setting_t *),
          len * sizeof(config_setting_t *));

  list->length--;

  if((list->capacity - list->length) >= CHUNK_SIZE)
  {
    /* realloc smaller? */
  }

  return(removed);
}

/* ------------------------------------------------------------------------- */

static void __config_setting_destroy(config_setting_t *setting)
{
  if(setting)
  {
    if(setting->name)
      _delete(setting->name);

    if(setting->type == CONFIG_TYPE_STRING)
      _delete(setting->value.sval);

    else if((setting->type == CONFIG_TYPE_GROUP)
            || (setting->type == CONFIG_TYPE_ARRAY)
            || (setting->type == CONFIG_TYPE_LIST))
    {
      if(setting->value.list)
        __config_list_destroy(setting->value.list);
    }

    if(setting->hook && setting->config->destructor)
      setting->config->destructor(setting->hook);

    _delete(setting);
  }
}

/* ------------------------------------------------------------------------- */

static void __config_list_destroy(config_list_t *list)
{
  config_setting_t **p;
  unsigned int i;

  if(! list)
    return;

  if(list->elements)
  {
    for(p = list->elements, i = 0; i < list->length; p++, i++)
      __config_setting_destroy(*p);

    _delete(list->elements);
  }

  _delete(list);
}

/* ------------------------------------------------------------------------- */

static int __config_vector_checktype(const config_setting_t *vector, int type)
{
  /* if the array is empty, then it has no type yet */

  if(! vector->value.list)
    return(CONFIG_TRUE);

  if(vector->value.list->length == 0)
    return(CONFIG_TRUE);

  /* if it's a list, any type is allowed */

  if(vector->type == CONFIG_TYPE_LIST)
    return(CONFIG_TRUE);

  /* otherwise the first element added determines the type of the array */

  return((vector->value.list->elements[0]->type == type)
         ? CONFIG_TRUE : CONFIG_FALSE);
}

/* ------------------------------------------------------------------------- */

static int __config_validate_name(const char *name)
{
  const char *p = name;

  if(*p == '\0')
    return(CONFIG_FALSE);

  if(! isalpha(*p) && (*p != '*'))
    return(CONFIG_FALSE);

  for(++p; *p; ++p)
  {
    if(! (isalpha(*p) || isdigit(*p) || strchr("*_-", (int)*p)))
      return(CONFIG_FALSE);
  }

  return(CONFIG_TRUE);
}

/* ------------------------------------------------------------------------- */

int config_read(config_t *config, FIOHANDLER stream)
{
  yyscan_t scanner;
  struct parse_context ctx;
  int r;

  /* Reinitialize the config (keep the destructor) */
  void (*destructor)(void *) = config->destructor;
  config_destroy(config);
  config_init(config);
  config->destructor = destructor;

  ctx.config = config;
  ctx.parent = config->root;
  ctx.setting = config->root;

  __config_locale_override();

  libconfig_yylex_init(&scanner);
  libconfig_yyrestart((FIOHANDLER)stream, scanner);
  r = libconfig_yyparse(scanner, &ctx);
  libconfig_yylex_destroy(scanner);

  __config_locale_restore();

  return(r == 0 ? CONFIG_TRUE : CONFIG_FALSE);
}

/* ------------------------------------------------------------------------- */

static void __config_write_setting(const config_setting_t *setting,
                                   FIOHANDLER stream, int depth)
{
#ifndef READONLY
  if(depth > 1)
    fprintf(stream, "%*s", (depth - 1) * 2, " ");

  if(setting->name)
  {
    fputs(setting->name, stream);
    fprintf(stream, " %c ", (setting->type == CONFIG_TYPE_GROUP ? ':' : '='));
  }

  __config_write_value(&(setting->value), setting->type, setting->format,
                       depth, stream);

  if(depth > 0)
  {
    fputc(';', stream);
    fputc('\n', stream);
  }
#endif
}

/* ------------------------------------------------------------------------- */

void config_write(const config_t *config, FIOHANDLER stream)
{
#ifndef READONLY
  __config_locale_override();

  __config_write_setting(config->root, stream, 0);

  __config_locale_restore();
#endif
}

/* ------------------------------------------------------------------------- */
int config_read_file(config_t *config, const char *filename)
{
  int ret;
  FIOHANDLER f = FIOOpen(filename, "rt");
  if(! f)
  {
    config->error_text = ___io_error;
    return(CONFIG_FALSE);
  }

  ret = config_read(config, f);
  FIOClose(f);
  return(ret);
}

/* ------------------------------------------------------------------------- */

int config_write_file(config_t *config, const char *filename)
{
#ifndef READONLY
  FIOHANDLER f = FIOOpen(filename, "wt");
  if(! f)
  {
    config->error_text = ___io_error;
    return(CONFIG_FALSE);
  }

  config_write(config, f);
  FIOClose(f);
#endif
 
  return(CONFIG_TRUE);
}

/* ------------------------------------------------------------------------- */

void config_destroy(config_t *config)
{
  __config_setting_destroy(config->root);

  memset((void *)config, 0, sizeof(config_t));
}

/* ------------------------------------------------------------------------- */

void config_init(config_t *config)
{
  memset((void *)config, 0, sizeof(config_t));

  config->root = _new(config_setting_t);
  config->root->type = CONFIG_TYPE_GROUP;
  config->root->config = config;
}

/* ------------------------------------------------------------------------- */

void config_set_auto_convert(config_t *config, int flag)
{
  if(flag)
    config->flags |= CONFIG_OPTION_AUTOCONVERT;
  else
    config->flags &= ~CONFIG_OPTION_AUTOCONVERT;
}

/* ------------------------------------------------------------------------- */

int config_get_auto_convert(const config_t *config)
{
  return((config->flags & CONFIG_OPTION_AUTOCONVERT) != 0);
}

/* ------------------------------------------------------------------------- */

static config_setting_t *config_setting_create(config_setting_t *parent,
                                               const char *name, int type)
{
  config_setting_t *setting;
  config_list_t *list;

  if((parent->type != CONFIG_TYPE_GROUP)
     && (parent->type != CONFIG_TYPE_ARRAY)
     && (parent->type != CONFIG_TYPE_LIST))
    return(NULL);

  setting = _new(config_setting_t);
  setting->parent = parent;
  setting->name = (name == NULL) ? NULL : strdup(name);
  setting->type = type;
  setting->config = parent->config;
  setting->hook = NULL;
  setting->line = 0;

  list = parent->value.list;

  if(! list)
    list = parent->value.list = _new(config_list_t);

  __config_list_add(list, setting);

  return(setting);
}

/* ------------------------------------------------------------------------- */

long config_setting_get_int(const config_setting_t *setting)
{
  switch(setting->type)
  {
    case CONFIG_TYPE_INT:
      return(setting->value.ival);

    case CONFIG_TYPE_INT64:
      if((setting->value.llval > INT32_MAX)
         || (setting->value.llval < INT32_MIN))
        return(0);
      else
        return((long)setting->value.llval);

    case CONFIG_TYPE_FLOAT:
      if((setting->config->flags & CONFIG_OPTION_AUTOCONVERT) != 0)
        return((long)(setting->value.fval));
      else
        /* fall through */;

    default:
      return(0);
  }
}

/* ------------------------------------------------------------------------- */

long long config_setting_get_int64(const config_setting_t *setting)
{
  switch(setting->type)
  {
    case CONFIG_TYPE_INT64:
      return(setting->value.llval);

    case CONFIG_TYPE_INT:
      return((long long)setting->value.ival);

    case CONFIG_TYPE_FLOAT:
      if((setting->config->flags & CONFIG_OPTION_AUTOCONVERT) != 0)
        return((long long)(setting->value.fval));
      else
        /* fall through */;

    default:
      return(0);
  }
}

/* ------------------------------------------------------------------------- */

int config_setting_lookup_int(const config_setting_t *setting,
                              const char *name, long *value)
{
  config_setting_t *member = config_setting_get_member(setting, name);
  if(! member)
    return(CONFIG_FALSE);

  if(config_setting_type(member) != CONFIG_TYPE_INT)
    return(CONFIG_FALSE);

  *value = config_setting_get_int(member);
  return(CONFIG_TRUE);
}

/* ------------------------------------------------------------------------- */

int config_setting_lookup_int64(const config_setting_t *setting,
                                const char *name, long long *value)
{
  config_setting_t *member = config_setting_get_member(setting, name);
  if(! member)
    return(CONFIG_FALSE);

  if(config_setting_type(member) != CONFIG_TYPE_INT64)
    return(CONFIG_FALSE);

  *value = config_setting_get_int64(member);
  return(CONFIG_TRUE);
}

/* ------------------------------------------------------------------------- */

int config_setting_lookup_float(const config_setting_t *setting,
                                const char *name, double *value)
{
  config_setting_t *member = config_setting_get_member(setting, name);
  if(! member)
    return(CONFIG_FALSE);

  if(config_setting_type(member) != CONFIG_TYPE_FLOAT)
    return(CONFIG_FALSE);

  *value = config_setting_get_float(member);
  return(CONFIG_TRUE);
}

/* ------------------------------------------------------------------------- */

int config_setting_lookup_string(const config_setting_t *setting,
                                 const char *name, const char **value)
{
  config_setting_t *member = config_setting_get_member(setting, name);
  if(! member)
    return(CONFIG_FALSE);

  if(config_setting_type(member) != CONFIG_TYPE_STRING)
    return(CONFIG_FALSE);

  *value = config_setting_get_string(member);
  return(CONFIG_TRUE);
}

/* ------------------------------------------------------------------------- */

int config_setting_lookup_bool(const config_setting_t *setting,
                               const char *name, int *value)
{
  config_setting_t *member = config_setting_get_member(setting, name);
  if(! member)
    return(CONFIG_FALSE);

  if(config_setting_type(member) != CONFIG_TYPE_BOOL)
    return(CONFIG_FALSE);

  *value = config_setting_get_bool(member);
  return(CONFIG_TRUE);
}

/* ------------------------------------------------------------------------- */

int config_setting_set_int(config_setting_t *setting, long value)
{
  switch(setting->type)
  {
    case CONFIG_TYPE_NONE:
      setting->type = CONFIG_TYPE_INT;
      /* fall through */

    case CONFIG_TYPE_INT:
      setting->value.ival = value;
      return(CONFIG_TRUE);

    case CONFIG_TYPE_FLOAT:
      if(config_get_auto_convert(setting->config))
      {
        setting->value.fval = (float)value;
        return(CONFIG_TRUE);
      }
      else
        return(CONFIG_FALSE);

    default:
      return(CONFIG_FALSE);
  }
}

/* ------------------------------------------------------------------------- */

int config_setting_set_int64(config_setting_t *setting, long long value)
{
  switch(setting->type)
  {
    case CONFIG_TYPE_NONE:
      setting->type = CONFIG_TYPE_INT64;
      /* fall through */

    case CONFIG_TYPE_INT64:
      setting->value.llval = value;
      return(CONFIG_TRUE);

    case CONFIG_TYPE_INT:
      if((value > INT32_MAX) || (value < INT32_MIN))
        setting->value.ival = 0;
      else
        setting->value.ival = (long)value;
      return(CONFIG_TRUE);

    case CONFIG_TYPE_FLOAT:
      if(config_get_auto_convert(setting->config))
      {
        setting->value.fval = (float)value;
        return(CONFIG_TRUE);
      }
      else
        return(CONFIG_FALSE);

    default:
      return(CONFIG_FALSE);
  }
}

/* ------------------------------------------------------------------------- */

double config_setting_get_float(const config_setting_t *setting)
{
  switch(setting->type)
  {
    case CONFIG_TYPE_FLOAT:
      return(setting->value.fval);

    case CONFIG_TYPE_INT:
      if(config_get_auto_convert(setting->config))
        return((double)(setting->value.ival));
      else
        return(0.0);

    case CONFIG_TYPE_INT64:
      if(config_get_auto_convert(setting->config))
        return((double)(setting->value.llval));
      else
        return(0.0);

    default:
      return(0.0);
  }
}

/* ------------------------------------------------------------------------- */

int config_setting_set_float(config_setting_t *setting, double value)
{
  switch(setting->type)
  {
    case CONFIG_TYPE_NONE:
      setting->type = CONFIG_TYPE_FLOAT;
      /* fall through */

    case CONFIG_TYPE_FLOAT:
      setting->value.fval = value;
      return(CONFIG_TRUE);

    case CONFIG_TYPE_INT:
      if((setting->config->flags & CONFIG_OPTION_AUTOCONVERT) != 0)
      {
        setting->value.ival = (long)value;
        return(CONFIG_TRUE);
      }
      else
        return(CONFIG_FALSE);

    case CONFIG_TYPE_INT64:
      if((setting->config->flags & CONFIG_OPTION_AUTOCONVERT) != 0)
      {
        setting->value.llval = (long long)value;
        return(CONFIG_TRUE);
      }
      else
        return(CONFIG_FALSE);

    default:
      return(CONFIG_FALSE);
  }
}

/* ------------------------------------------------------------------------- */

int config_setting_get_bool(const config_setting_t *setting)
{
  return((setting->type == CONFIG_TYPE_BOOL) ? setting->value.ival : 0);
}

/* ------------------------------------------------------------------------- */

int config_setting_set_bool(config_setting_t *setting, int value)
{
  if(setting->type == CONFIG_TYPE_NONE)
    setting->type = CONFIG_TYPE_BOOL;
  else if(setting->type != CONFIG_TYPE_BOOL)
    return(CONFIG_FALSE);

  setting->value.ival = value;
  return(CONFIG_TRUE);
}

/* ------------------------------------------------------------------------- */

const char *config_setting_get_string(const config_setting_t *setting)
{
  return((setting->type == CONFIG_TYPE_STRING) ? setting->value.sval : NULL);
}

/* ------------------------------------------------------------------------- */

int config_setting_set_string(config_setting_t *setting, const char *value)
{
  if(setting->type == CONFIG_TYPE_NONE)
    setting->type = CONFIG_TYPE_STRING;
  else if(setting->type != CONFIG_TYPE_STRING)
    return(CONFIG_FALSE);

  if(setting->value.sval)
    _delete(setting->value.sval);

  setting->value.sval = (value == NULL) ? NULL : strdup(value);
  return(CONFIG_TRUE);
}

/* ------------------------------------------------------------------------- */

int config_setting_set_format(config_setting_t *setting, short format)
{
  if(((setting->type != CONFIG_TYPE_INT)
      && (setting->type != CONFIG_TYPE_INT64))
     || ((format != CONFIG_FORMAT_DEFAULT) && (format != CONFIG_FORMAT_HEX)))
    return(CONFIG_FALSE);

  setting->format = format;

  return(CONFIG_TRUE);
}

/* ------------------------------------------------------------------------- */

short config_setting_get_format(config_setting_t *setting)
{
  return(setting->format);
}

/* ------------------------------------------------------------------------- */

config_setting_t *config_lookup(const config_t *config, const char *path)
{
  const char *p = path;
  config_setting_t *setting = config->root, *found;

  for(;;)
  {
    while(*p && strchr(PATH_TOKENS, *p))
      p++;

    if(! *p)
      break;

    if(*p == '[')
      found = config_setting_get_elem(setting, atoi(++p));
    else
      found = config_setting_get_member(setting, p);

    if(! found)
      break;

    setting = found;

    while(! strchr(PATH_TOKENS, *p))
      p++;
  }

  return(*p ? NULL : setting);
}

/* ------------------------------------------------------------------------- */

int config_lookup_string(const config_t *config, const char *path,
                         const char **value)
{
  const config_setting_t *s = config_lookup(config, path);
  if(! s)
    return(CONFIG_FALSE);

  if(config_setting_type(s) != CONFIG_TYPE_STRING)
    return(CONFIG_FALSE);

  *value = config_setting_get_string(s);

  return(CONFIG_TRUE);
}

/* ------------------------------------------------------------------------- */

int config_lookup_int(const config_t *config, const char *path,
                      long *value)
{
  const config_setting_t *s = config_lookup(config, path);
  if(! s)
    return(CONFIG_FALSE);

  if(config_setting_type(s) != CONFIG_TYPE_INT)
    return(CONFIG_FALSE);

  *value = config_setting_get_int(s);

  return(CONFIG_TRUE);
}

/* ------------------------------------------------------------------------- */

int config_lookup_int64(const config_t *config, const char *path,
                        long long *value)
{
  const config_setting_t *s = config_lookup(config, path);
  if(! s)
    return(CONFIG_FALSE);

  if(config_setting_type(s) != CONFIG_TYPE_INT64)
    return(CONFIG_FALSE);

  *value = config_setting_get_int64(s);

  return(CONFIG_TRUE);
}

/* ------------------------------------------------------------------------- */

int config_lookup_float(const config_t *config, const char *path,
                        double *value)
{
  const config_setting_t *s = config_lookup(config, path);
  if(! s)
    return(CONFIG_FALSE);

  if(config_setting_type(s) != CONFIG_TYPE_FLOAT)
    return(CONFIG_FALSE);

  *value = config_setting_get_float(s);
  return(CONFIG_TRUE);
}

/* ------------------------------------------------------------------------- */

int config_lookup_bool(const config_t *config, const char *path, int *value)
{
  const config_setting_t *s = config_lookup(config, path);
  if(! s)
    return(CONFIG_FALSE);

  if(config_setting_type(s) != CONFIG_TYPE_BOOL)
    return(CONFIG_FALSE);

  *value = config_setting_get_bool(s);
  return(CONFIG_TRUE);
}

/* ------------------------------------------------------------------------- */

long config_setting_get_int_elem(const config_setting_t *vector, int idx)
{
  const config_setting_t *element = config_setting_get_elem(vector, idx);

  return(element ? config_setting_get_int(element) : 0);
}

/* ------------------------------------------------------------------------- */

config_setting_t *config_setting_set_int_elem(config_setting_t *vector,
                                              int idx, long value)
{
  config_setting_t *element = NULL;

  if((vector->type != CONFIG_TYPE_ARRAY) && (vector->type != CONFIG_TYPE_LIST))
    return(NULL);

  if(idx < 0)
  {
    if(! __config_vector_checktype(vector, CONFIG_TYPE_INT))
      return(NULL);

    element = config_setting_create(vector, NULL, CONFIG_TYPE_INT);
  }
  else
  {
    element = config_setting_get_elem(vector, idx);

    if(! element)
      return(NULL);
  }

  if(! config_setting_set_int(element, value))
    return(NULL);

  return(element);
}

/* ------------------------------------------------------------------------- */

long long config_setting_get_int64_elem(const config_setting_t *vector,
                                        int idx)
{
  const config_setting_t *element = config_setting_get_elem(vector, idx);

  return(element ? config_setting_get_int64(element) : 0);
}

/* ------------------------------------------------------------------------- */

config_setting_t *config_setting_set_int64_elem(config_setting_t *vector,
                                                int idx, long long value)
{
  config_setting_t *element = NULL;

  if((vector->type != CONFIG_TYPE_ARRAY) && (vector->type != CONFIG_TYPE_LIST))
    return(NULL);

  if(idx < 0)
  {
    if(! __config_vector_checktype(vector, CONFIG_TYPE_INT64))
      return(NULL);

    element = config_setting_create(vector, NULL, CONFIG_TYPE_INT64);
  }
  else
  {
    element = config_setting_get_elem(vector, idx);

    if(! element)
      return(NULL);
  }

  if(! config_setting_set_int64(element, value))
    return(NULL);

  return(element);
}

/* ------------------------------------------------------------------------- */

double config_setting_get_float_elem(const config_setting_t *vector, int idx)
{
  config_setting_t *element = config_setting_get_elem(vector, idx);

  return(element ? config_setting_get_float(element) : 0.0);
}

/* ------------------------------------------------------------------------- */

config_setting_t *config_setting_set_float_elem(config_setting_t *vector,
                                                int idx, double value)
{
  config_setting_t *element = NULL;

  if((vector->type != CONFIG_TYPE_ARRAY) && (vector->type != CONFIG_TYPE_LIST))
    return(NULL);

  if(idx < 0)
  {
    if(! __config_vector_checktype(vector, CONFIG_TYPE_FLOAT))
      return(NULL);

    element = config_setting_create(vector, NULL, CONFIG_TYPE_FLOAT);
  }
  else
    element = config_setting_get_elem(vector, idx);

  if(! element)
    return(NULL);

  if(! config_setting_set_float(element, value))
    return(NULL);

  return(element);
}

/* ------------------------------------------------------------------------- */

int config_setting_get_bool_elem(const config_setting_t *vector, int idx)
{
  config_setting_t *element = config_setting_get_elem(vector, idx);

  if(! element)
    return(CONFIG_FALSE);

  if(element->type != CONFIG_TYPE_BOOL)
    return(CONFIG_FALSE);

  return(element->value.ival);
}

/* ------------------------------------------------------------------------- */

config_setting_t *config_setting_set_bool_elem(config_setting_t *vector,
                                               int idx, int value)
{
  config_setting_t *element = NULL;

  if((vector->type != CONFIG_TYPE_ARRAY) && (vector->type != CONFIG_TYPE_LIST))
    return(NULL);

  if(idx < 0)
  {
    if(! __config_vector_checktype(vector, CONFIG_TYPE_BOOL))
      return(NULL);

    element = config_setting_create(vector, NULL, CONFIG_TYPE_BOOL);
  }
  else
    element = config_setting_get_elem(vector, idx);

  if(! element)
    return(NULL);

  if(! config_setting_set_bool(element, value))
    return(NULL);

  return(element);
}

/* ------------------------------------------------------------------------- */

const char *config_setting_get_string_elem(const config_setting_t *vector,
                                           int idx)
{
  config_setting_t *element = config_setting_get_elem(vector, idx);

  if(! element)
    return(NULL);

  if(element->type != CONFIG_TYPE_STRING)
    return(NULL);

  return(element->value.sval);
}

/* ------------------------------------------------------------------------- */

config_setting_t *config_setting_set_string_elem(config_setting_t *vector,
                                                 int idx, const char *value)
{
  config_setting_t *element = NULL;

  if((vector->type != CONFIG_TYPE_ARRAY) && (vector->type != CONFIG_TYPE_LIST))
    return(NULL);

  if(idx < 0)
  {
    if(! __config_vector_checktype(vector, CONFIG_TYPE_STRING))
      return(NULL);

    element = config_setting_create(vector, NULL, CONFIG_TYPE_STRING);
  }
  else
    element = config_setting_get_elem(vector, idx);

  if(! element)
    return(NULL);

  if(! config_setting_set_string(element, value))
    return(NULL);

  return(element);
}

/* ------------------------------------------------------------------------- */

config_setting_t *config_setting_get_elem(const config_setting_t *vector,
                                          unsigned int idx)
{
  config_list_t *list = vector->value.list;

  if(((vector->type != CONFIG_TYPE_ARRAY)
      && (vector->type != CONFIG_TYPE_LIST)
      && (vector->type != CONFIG_TYPE_GROUP)) || ! list)
    return(NULL);

  if(idx >= list->length)
    return(NULL);

  return(list->elements[idx]);
}

/* ------------------------------------------------------------------------- */

config_setting_t *config_setting_get_member(const config_setting_t *setting,
                                            const char *name)
{
  if(setting->type != CONFIG_TYPE_GROUP)
    return(NULL);

  return(__config_list_search(setting->value.list, name, NULL));
}

/* ------------------------------------------------------------------------- */

void config_set_destructor(config_t *config, void (*destructor)(void *))
{
  config->destructor = destructor;
}

/* ------------------------------------------------------------------------- */

int config_setting_length(const config_setting_t *setting)
{
  if((setting->type != CONFIG_TYPE_GROUP)
     && (setting->type != CONFIG_TYPE_ARRAY)
     && (setting->type != CONFIG_TYPE_LIST))
    return(0);

  if(! setting->value.list)
    return(0);

  return(setting->value.list->length);
}

/* ------------------------------------------------------------------------- */

void config_setting_set_hook(config_setting_t *setting, void *hook)
{
  setting->hook = hook;
}

/* ------------------------------------------------------------------------- */

config_setting_t *config_setting_add(config_setting_t *parent,
                                     const char *name, int type)
{
  if((type < CONFIG_TYPE_NONE) || (type > CONFIG_TYPE_LIST))
    return(NULL);

  if(! parent)
    return(NULL);

  if((parent->type == CONFIG_TYPE_ARRAY) || (parent->type == CONFIG_TYPE_LIST))
    name = NULL;

  if(name)
  {
    if(! __config_validate_name(name))
      return(NULL);
  }

  if(config_setting_get_member(parent, name) != NULL)
    return(NULL); /* already exists */

  return(config_setting_create(parent, name, type));
}

/* ------------------------------------------------------------------------- */

int config_setting_remove(config_setting_t *parent, const char *name)
{
  unsigned int idx;
  config_setting_t *setting;

  if(! parent)
    return(CONFIG_FALSE);

  if(parent->type != CONFIG_TYPE_GROUP)
    return(CONFIG_FALSE);

  if(! (setting = __config_list_search(parent->value.list, name, &idx)))
    return(CONFIG_FALSE);

  __config_list_remove(parent->value.list, idx);
  __config_setting_destroy(setting);

  return(CONFIG_TRUE);
}

/* ------------------------------------------------------------------------- */

int config_setting_remove_elem(config_setting_t *parent, unsigned int idx)
{
  config_list_t *list;
  config_setting_t *removed = NULL;

  if(! parent)
    return(CONFIG_FALSE);

  list = parent->value.list;

  if(((parent->type != CONFIG_TYPE_ARRAY)
      && (parent->type != CONFIG_TYPE_LIST)
      && (parent->type != CONFIG_TYPE_GROUP)) || ! list)
    return(CONFIG_FALSE);

  if(idx >= list->length)
    return(CONFIG_FALSE);

  removed = __config_list_remove(list, idx);
  __config_setting_destroy(removed);

  return(CONFIG_TRUE);
}

/* ------------------------------------------------------------------------- */

int config_setting_index(const config_setting_t *setting)
{
  config_setting_t **found = NULL;
  config_list_t *list;
  int i;

  if(! setting->parent)
    return(-1);

  list = setting->parent->value.list;

  for(i = 0, found = list->elements; i < list->length; ++i, ++found)
  {
    if(*found == setting)
      return(i);
  }

  return(-1);
}

/* ------------------------------------------------------------------------- */
/* eof */
