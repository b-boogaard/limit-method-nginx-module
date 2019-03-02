cmake_minimum_required(VERSION 3.13)
project(limit_method_nginx_module C)

set(CMAKE_C_STANDARD 99)

include_directories(
    ${PROJECT_SOURCE_DIR}/../nginx/src/core
    ${PROJECT_SOURCE_DIR}/../nginx/src/http
)
add_library(limit_method_nginx_module src/limit_method_nginx_module.c src/limit_method_nginx_module.h)

