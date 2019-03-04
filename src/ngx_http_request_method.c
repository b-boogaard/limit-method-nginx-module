/*!
 * \file ngx_http_request_method.c
 * \brief Manages configured request methods for ngx_http_limit_method_module.
 *
 * \author Brian van de Boogaard (b-boogaard)
 * \copyright Copyright (c) 2019 Brian van de Boogaard (b-boogaard). All rights reserved.
 */

#include <ngx_http.h>
#include <stdio.h>
#include "ngx_http_request_method.h"

static const ngx_str_t get       = ngx_string("GET");
static const ngx_str_t head      = ngx_string("HEAD");
static const ngx_str_t post      = ngx_string("POST");
static const ngx_str_t put       = ngx_string("PUT");
static const ngx_str_t delete    = ngx_string("DELETE");
static const ngx_str_t mkcol     = ngx_string("MKCOL");
static const ngx_str_t copy      = ngx_string("COPY");
static const ngx_str_t move      = ngx_string("MOVE");
static const ngx_str_t options   = ngx_string("OPTIONS");
static const ngx_str_t propfind  = ngx_string("PROPFIND");
static const ngx_str_t proppatch = ngx_string("PROPPATCH");
static const ngx_str_t lock      = ngx_string("LOCK");
static const ngx_str_t unlock    = ngx_string("UNLOCK");
static const ngx_str_t patch     = ngx_string("PATCH");
static const ngx_str_t trace     = ngx_string("TRACE");

static ngx_uint_t request_method_get_number (ngx_str_t method);

extern ngx_int_t
ngx_http_request_method_contains_number (ngx_array_t method_numbers, ngx_uint_t method_number)
{
  ngx_uint_t i;
  for (i = 0; i < method_numbers.nelts; i++)
    {
      ngx_uint_t * current_number_ptr = (ngx_uint_t *) method_numbers.elts + i;
      if (*current_number_ptr == method_number)
        {
          return 1;
        }
    }

  return 0;
}

extern ngx_array_t *
ngx_http_request_method_names_to_numbers (ngx_conf_t * cf, ngx_array_t method_names)
{
  ngx_array_t * method_numbers = ngx_array_create (cf->pool, 20, sizeof (ngx_uint_t));
  ngx_array_init (method_numbers, cf->pool, 20, sizeof (ngx_uint_t));

  ngx_uint_t i;
  for (i = 0; i < method_names.nelts; i++)
    {
      ngx_str_t * method_name = (ngx_str_t *) method_names.elts + i;
      ngx_uint_t method_number = request_method_get_number (*method_name);

      ngx_uint_t * array_ptr = ngx_array_push (method_numbers);
      if (array_ptr == NULL)
        {
          return NULL;
        }

      *array_ptr = method_number;
    }

  return method_numbers;
}

static ngx_uint_t
request_method_get_number (const ngx_str_t method)
{
  if (ngx_strcasecmp (get.data, method.data) == 0)
    return NGX_HTTP_GET;
  else if (ngx_strcasecmp (head.data, method.data) == 0)
    return NGX_HTTP_HEAD;
  else if (ngx_strcasecmp (post.data, method.data) == 0)
    return NGX_HTTP_POST;
  else if (ngx_strcasecmp (put.data, method.data) == 0)
    return NGX_HTTP_PUT;
  else if (ngx_strcasecmp (delete.data, method.data) == 0)
    return NGX_HTTP_DELETE;
  else if (ngx_strcasecmp (mkcol.data, method.data) == 0)
    return NGX_HTTP_MKCOL;
  else if (ngx_strcasecmp (copy.data, method.data) == 0)
    return NGX_HTTP_COPY;
  else if (ngx_strcasecmp (move.data, method.data) == 0)
    return NGX_HTTP_MOVE;
  else if (ngx_strcasecmp (options.data, method.data) == 0)
    return NGX_HTTP_OPTIONS;
  else if (ngx_strcasecmp (propfind.data, method.data) == 0)
    return NGX_HTTP_PROPFIND;
  else if (ngx_strcasecmp (proppatch.data, method.data) == 0)
    return NGX_HTTP_PROPPATCH;
  else if (ngx_strcasecmp (lock.data, method.data) == 0)
    return NGX_HTTP_LOCK;
  else if (ngx_strcasecmp (unlock.data, method.data) == 0)
    return NGX_HTTP_UNLOCK;
  else if (ngx_strcasecmp (patch.data, method.data) == 0)
    return NGX_HTTP_PATCH;
  else if (ngx_strcasecmp (trace.data, method.data) == 0)
    return NGX_HTTP_TRACE;
  else
    return NGX_HTTP_UNKNOWN;
}
