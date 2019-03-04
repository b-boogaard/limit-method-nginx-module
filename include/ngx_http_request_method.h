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

extern ngx_int_t
ngx_http_request_method_contains_number (ngx_array_t method_numbers, ngx_uint_t method_number);

extern ngx_array_t *
ngx_http_request_method_names_to_numbers (ngx_conf_t * cf, ngx_array_t method_names);

#endif //LIMIT_METHOD_NGINX_MODULE_NGX_HTTP_REQUEST_METHOD_HASH_H
