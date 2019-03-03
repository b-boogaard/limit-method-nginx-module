/*!
 * \file ngx_http_request_method_hash.h
 * \author Brian van de Boogaard (b-boogaard)
 * \copyright Copyright (c) 2019 Brian van de Boogaard (b-boogaard). All rights reserved.
 */

#ifndef LIMIT_METHOD_NGINX_MODULE_NGX_HTTP_REQUEST_METHOD_HASH_H
#define LIMIT_METHOD_NGINX_MODULE_NGX_HTTP_REQUEST_METHOD_HASH_H

#include <ngx_core.h>
#include <ngx_config.h>
#include <ngx_http.h>

extern ngx_hash_t * ngx_http_request_method_create_hash (ngx_conf_t * cf, ngx_array_t methods);
extern ngx_int_t ngx_http_request_method_get_value (ngx_hash_t hash, ngx_str_t key_value);

#endif //LIMIT_METHOD_NGINX_MODULE_NGX_HTTP_REQUEST_METHOD_HASH_H
