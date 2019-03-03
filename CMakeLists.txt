cmake_minimum_required(VERSION 3.13)
project(limit_method_nginx_module C)

set(CMAKE_C_STANDARD 99)

include(GNUInstallDirs)
include(ExternalProject)
include_directories(/usr/local/include)
include_directories(include)

add_library(limit_method_nginx_module src/ngx_http_limit_method_module.c include/ngx_http_limit_method_module.h src/ngx_http_request_method_hash.c include/ngx_http_request_method_hash.h src/ngx_request_method_set.c include/ngx_request_method_set.h)

## Documentation.
# Require dot, treat the other components as optional
find_package(Doxygen
    REQUIRED dot
    OPTIONAL_COMPONENTS mscgen dia)

if (DOXYGEN_FOUND)
  set(DOXYGEN_GENERATE_HTML YES)
  set(DOXYGEN_GENERATE_MAN YES)
  set(DOXYGEN_OPTIMIZE_OUTPUT_FOR_C YES)
  set(DOXYGEN_QT_AUTOBRIEF YES)
  set(DOXYGEN_EXTRACT_ALL YES)
  set(DOXYGEN_EXCLUDE_PATTERNS */cmake-* */cmake-*/*)
  set(DOXYGEN_QUIET YES)

  doxygen_add_docs(
      docs
      ${PROJECT_SOURCE_DIR}
      COMMENT "Building documentation using doxygen."
  )

else (DOXYGEN_FOUND)
  message("Doxygen needs to be installed to generate the doxygen documentation")
endif (DOXYGEN_FOUND)

# Make this better.
# Add ExternalProject for pcre which is also required by nginx.
find_package(Hg)
#find_package(OpenSSL)
#find_package(ZLIB)

if (HG_FOUND)
  set (NGINX_BUILD_DIR ${PROJECT_SOURCE_DIR}/nginx-build)
  set (NGINX_EXTERN_DIR ${PROJECT_SOURCE_DIR}/extern/src/nginx)

  ExternalProject_Add(nginx
      PREFIX ${PROJECT_SOURCE_DIR}/extern
      HG_REPOSITORY http://hg.nginx.org/nginx/
      HG_TAG release-1.15.9
      UPDATE_COMMAND hg pull
      COMMENT "Building nginx"
      CONFIGURE_COMMAND cd ${NGINX_EXTERN_DIR} && ./auto/configure --with-debug --prefix=${NGINX_BUILD_DIR} --add-module=${PROJECT_SOURCE_DIR}
      BUILD_IN_SOURCE YES
      BUILD_COMMAND cd ${NGINX_EXTERN_DIR} && make
      INSTALL_COMMAND cd ${NGINX_EXTERN_DIR} && make install
  )

  configure_file(
      ${PROJECT_SOURCE_DIR}/templates/nginx.conf
      ${NGINX_BUILD_DIR}/conf/nginx.conf
      COPYONLY
  )

  ExternalProject_Get_Property(nginx source_dir)
  include_directories(${source_dir}/src/http)
  include_directories(${source_dir}/objs)
  include_directories(${source_dir}/src/http/modules)
  include_directories(${source_dir}/src/event/modules)
  include_directories(${source_dir}/src/os/unix)
  include_directories(${source_dir}/src/core)
  include_directories(${source_dir}/src/event)

else (HG_FOUND)
  message("Mercurial required for installing external project nginx.")
#  message("OpenSSL required for installing external project nginx.")
#  message("Zlib required for installing external project nginx.")
endif (HG_FOUND)

