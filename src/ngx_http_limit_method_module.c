/*!
 * \file ngx_http_limit_method_module.c
 * \brief Nginx module that allows limiting locations by request method with a fallback.
 *
 * \author Brian van de Boogaard (b-boogaard)
 * \copyright Copyright (c) 2019 Brian van de Boogaard (b-boogaard). All rights reserved.
 */

#include <ngx_config.h>
#include <ngx_core.h>
#include <ngx_http.h>
#include <stddef.h>
#include <string.h>
#include "ngx_http_limit_method_module.h"
#include "ngx_http_request_method.h"

typedef struct {
  ngx_flag_t enabled;
  ngx_str_t  fallback;
  ngx_array_t * method_names;
  ngx_array_t * method_numbers;
} ngx_http_limit_method_loc_conf_t;

static ngx_int_t ngx_http_limit_method_handler (ngx_http_request_t * r);
static void * ngx_http_limit_method_create_loc_conf (ngx_conf_t * cf);
static char * ngx_http_limit_method_merge_loc_conf (ngx_conf_t * cf, void * parent, void * child);
static ngx_int_t ngx_http_limit_method_init (ngx_conf_t * cf);

static ngx_command_t ngx_http_limit_method_commands[] = {
    {ngx_string("limit_method"),
     NGX_HTTP_LOC_CONF | NGX_CONF_FLAG,
     ngx_conf_set_flag_slot,
     NGX_HTTP_LOC_CONF_OFFSET,
     offsetof (ngx_http_limit_method_loc_conf_t, enabled),
     NULL
    },
    {ngx_string("limit_method_allowed_methods"),
     NGX_HTTP_LOC_CONF | NGX_CONF_1MORE,
     ngx_conf_set_str_array_slot,
     NGX_HTTP_LOC_CONF_OFFSET,
     offsetof (ngx_http_limit_method_loc_conf_t, method_names),
     NULL
    },
    {ngx_string("limit_method_fallback"),
     NGX_HTTP_LOC_CONF | NGX_CONF_TAKE1,
     ngx_conf_set_str_slot,
     NGX_HTTP_LOC_CONF_OFFSET,
     offsetof (ngx_http_limit_method_loc_conf_t, fallback),
     NULL
    },

    ngx_null_command
};

static ngx_http_module_t ngx_http_limit_method_ctx = {
    NULL,                                       /* pre-configuration */
    ngx_http_limit_method_init,                 /* post-configuration */

    NULL,                                       /* create main configuration */
    NULL,                                       /* init main configuration */

    NULL,                                       /* create server configuration */
    NULL,                                       /* merge server configuration */

    ngx_http_limit_method_create_loc_conf,      /* create location configuration */
    ngx_http_limit_method_merge_loc_conf        /* merge location configuration */
};

ngx_module_t ngx_http_limit_method_module = {
    NGX_MODULE_V1,
    &ngx_http_limit_method_ctx,        /* module context */
    ngx_http_limit_method_commands,    /* module directives */
    NGX_HTTP_MODULE,                   /* module type */
    NULL,                              /* init master */
    NULL,                              /* init module */
    NULL,                              /* init process */
    NULL,                              /* init thread */
    NULL,                              /* exit thread */
    NULL,                              /* exit process */
    NULL,                              /* exit master */
    NGX_MODULE_V1_PADDING
};

static ngx_int_t
ngx_http_limit_method_handler (ngx_http_request_t * r)
{
  ngx_http_limit_method_loc_conf_t * lmcf;
  lmcf = ngx_http_get_module_loc_conf (r, ngx_http_limit_method_module);

  if (lmcf->enabled == 0)
    {
      return NGX_DECLINED;
    }

  ngx_int_t is_allowed = ngx_http_request_method_contains_number (*lmcf->method_numbers, r->method);

  // method isn't allowed for this location; do internal redirect to fallback.
  if (is_allowed == 0)
    {
      ngx_int_t result = ngx_http_named_location (r, &lmcf->fallback);
      ngx_http_finalize_request (r, NGX_DONE);

      return result;
    }

  return NGX_DECLINED;
}

static void *
ngx_http_limit_method_create_loc_conf (ngx_conf_t * cf)
{
  ngx_http_limit_method_loc_conf_t * conf;

  conf = ngx_pcalloc (cf->pool, sizeof (ngx_http_limit_method_loc_conf_t));
  if (conf == NULL)
    {
      return NGX_CONF_ERROR;
    }

  conf->method_names   = NGX_CONF_UNSET_PTR;
  conf->method_numbers = NGX_CONF_UNSET_PTR;
  conf->enabled        = NGX_CONF_UNSET;

  return conf;
}

static char *
ngx_http_limit_method_merge_loc_conf (ngx_conf_t * cf, void * parent, void * child)
{
  ngx_http_limit_method_loc_conf_t * prev = parent;
  ngx_http_limit_method_loc_conf_t * conf = child;

  ngx_array_t * default_methods = ngx_array_create (cf->pool, 10, sizeof (ngx_str_t));
  ngx_array_init (default_methods, cf->pool, 10, sizeof (ngx_str_t));

  ngx_conf_merge_value(conf->enabled, prev->enabled, 0);
  ngx_conf_merge_str_value(conf->fallback, prev->fallback, "");
  ngx_conf_merge_ptr_value(conf->method_names, prev->method_names, default_methods);

  conf->method_numbers = ngx_http_request_method_names_to_numbers (cf, *conf->method_names);

  if (conf->method_numbers == NULL)
    {
      return NGX_CONF_ERROR;
    }

  return NGX_CONF_OK;
}

static ngx_int_t
ngx_http_limit_method_init (ngx_conf_t * cf)
{
  ngx_http_handler_pt       * h;
  ngx_http_core_main_conf_t * cmcf;

  cmcf = ngx_http_conf_get_module_main_conf(cf, ngx_http_core_module);

  h = ngx_array_push (&cmcf->phases[NGX_HTTP_ACCESS_PHASE].handlers);
  if (h == NULL)
    {
      return NGX_ERROR;
    }

  *h = ngx_http_limit_method_handler;

  return NGX_OK;
}

