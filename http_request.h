/**
 * @file Main HTTP request header
 */
#ifndef __HTTP_REQUEST_H__
#define __HTTP_REQUEST_H__

#include "str_view.h"
#include "types.h"

/**
 * @brief Supported HTTP methods
 */
enum http_request_method {
    hrm_get,
    hrm_unsupported
};

/**
 * @brief HTTP request structure
 */
struct http_request {
    enum http_request_method  method;
    struct str_view           requested_file;
};

/**
 * @brief Read request from socket
 */
i32 http_request__read(struct http_request* req, i32 fd);

#endif //__HTTP_REQUEST_H__
