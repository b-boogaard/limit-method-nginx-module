/*!
 * \file ngx_http_request_method_hash.c
 * \author Brian van de Boogaard (b-boogaard)
 * \copyright Copyright (c) 2019 Brian van de Boogaard (b-boogaard). All rights reserved.
 */

#include <ngx_http.h>
#include "ngx_http_request_method_hash.h"

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

static ngx_hash_keys_arrays_t * init_request_method_keys (ngx_conf_t * cf);
static ngx_int_t get_request_method_number (ngx_str_t method);
static ngx_hash_keys_arrays_t * create_request_method_keys (ngx_array_t methods, ngx_conf_t * cf);

extern ngx_hash_t *
ngx_http_request_method_create_hash (ngx_conf_t * cf, const ngx_array_t methods)
{
  ngx_hash_t * method_hash = (ngx_hash_t *) ngx_palloc (cf->pool, sizeof *method_hash);

  // Setup init hash.
  ngx_hash_init_t hash;
  hash.hash        = method_hash;
  hash.key         = ngx_hash_key;
  hash.max_size    = methods.nelts + 5;
  hash.bucket_size = ngx_align(64, ngx_cacheline_size);
  hash.name        = "method_hash";
  hash.pool        = cf->pool;
  hash.temp_pool   = cf->temp_pool;


  // Create keys.
  ngx_hash_keys_arrays_t * method_keys = create_request_method_keys (methods, cf);

  ngx_hash_init (&hash, method_keys->keys.elts, method_keys->keys.nelts);

  return method_hash;
}

static ngx_hash_keys_arrays_t *
init_request_method_keys (ngx_conf_t * cf)
{
  // Create method_keys array.
  ngx_hash_keys_arrays_t * method_keys = ngx_palloc (cf->pool, sizeof *method_keys);
  method_keys->pool      = cf->pool;
  method_keys->temp_pool = cf->temp_pool;

  ngx_hash_keys_array_init (method_keys, NGX_HASH_SMALL);

  return method_keys;
}

static ngx_int_t
get_request_method_number (const ngx_str_t method)
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

static ngx_hash_keys_arrays_t *
create_request_method_keys (ngx_array_t methods, ngx_conf_t * cf)
{
  ngx_hash_keys_arrays_t * method_keys = init_request_method_keys (cf);

  ngx_uint_t i;
  for (i = 0; i < methods.nelts; i++)
    {
      ngx_str_t * method = (ngx_str_t *) methods.elts + i;
      ngx_int_t method_number = get_request_method_number (*method);

      if (method_number == NGX_HTTP_UNKNOWN)
        continue;

      ngx_hash_add_key (method_keys, method, (void *) method_number, NGX_HASH_READONLY_KEY);
    }

  return method_keys;
}
