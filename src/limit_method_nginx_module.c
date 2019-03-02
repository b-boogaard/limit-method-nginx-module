#include <ngx_config.h>
#include <ngx_core.h>
#include <ngx_http.h>
#include <stddef.h>
#include "limit_method_nginx_module.h"

typedef unsigned char u_char;

typedef struct {
  ngx_str_t  fallback;
} ngx_http_limit_method_loc_conf_t;

static ngx_int_t ngx_http_limit_method_handler(ngx_http_request_t *r);
static void *ngx_http_limit_method_create_loc_conf(ngx_conf_t *cf);
static char *ngx_http_limit_method_merge_loc_conf(ngx_conf_t *cf, void *parent, void *child);
static ngx_int_t ngx_http_limit_method_init(ngx_conf_t *cf);


static ngx_command_t  ngx_http_limit_method_commands[] = {
    { ngx_string("limit_method"),
      NGX_HTTP_LOC_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_str_slot,
      NGX_HTTP_LOC_CONF_OFFSET,
      0,
      NULL
    },

    ngx_null_command
};


static ngx_http_module_t  ngx_http_limit_method_ctx = {
    NULL,                                       /* preconfiguration */
    ngx_http_limit_method_init,                 /* postconfiguration */

    NULL,                                       /* create main configuration */
    NULL,                                       /* init main configuration */

    NULL,                                       /* create server configuration */
    NULL,                                       /* merge server configuration */

    ngx_http_limit_method_create_loc_conf,      /* create location configuration */
    ngx_http_limit_method_merge_loc_conf        /* merge location configuration */
};


ngx_module_t  ngx_http_limit_method_module = {
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
ngx_http_limit_method_handler(ngx_http_request_t *r)
{
    ngx_http_limit_method_loc_conf_t  *lmcf;
    lmcf = ngx_http_get_module_loc_conf(r, ngx_http_limit_method_module);

    if (lmcf->fallback.len <= 1) {
        return NGX_DECLINED;
    }

    ngx_int_t result = ngx_http_named_location(r, &lmcf->fallback);

    ngx_http_finalize_request (r, NGX_DONE);

    return result;
}


static void *
ngx_http_limit_method_create_loc_conf(ngx_conf_t *cf)
{
    ngx_http_limit_method_loc_conf_t  *conf;

    conf = ngx_pcalloc(cf->pool, sizeof(ngx_http_limit_method_loc_conf_t));
    if (conf == NULL) {
        return NULL;
    }

    /*
     * set by ngx_pcalloc():
     *
     *      conf->fallback = { 0, NULL };
     */

    return conf;
}


static char *
ngx_http_limit_method_merge_loc_conf(ngx_conf_t *cf, void *parent, void *child)
{
    ngx_http_limit_method_loc_conf_t *prev = parent;
    ngx_http_limit_method_loc_conf_t *conf = child;

    ngx_conf_merge_str_value(conf->fallback, prev->fallback, "");

    return NGX_CONF_OK;
}


static ngx_int_t
ngx_http_limit_method_init(ngx_conf_t *cf)
{
    ngx_http_handler_pt        *h;
    ngx_http_core_main_conf_t  *cmcf;

    cmcf = ngx_http_conf_get_module_main_conf(cf, ngx_http_core_module);

    h = ngx_array_push(&cmcf->phases[NGX_HTTP_ACCESS_PHASE].handlers);
    if (h == NULL) {
        return NGX_ERROR;
    }

    *h = ngx_http_limit_method_handler;

    return NGX_OK;
}