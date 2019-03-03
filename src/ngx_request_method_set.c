/*!
 * \file ngx_request_method_set.c
 * \author Brian van de Boogaard
 * \copyright Copyright (c) 2019 Brian van de Boogaard. All rights reserved.
 */

#include <ngx_config.h>
#include <ngx_core.h>
#include <ngx_rbtree.h>
#include "ngx_request_method_set.h"

typedef struct {
  ngx_rbtree_t rbtree;
  ngx_rbtree_node_t sentinel;
} request_method_tree_t;

typedef struct {
  ngx_rbtree_node_t rbnode;
} request_method_node_t;
