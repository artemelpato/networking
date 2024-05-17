/**
 * @file Main HTTP server header
 */

#ifndef __HTTP_SERVER_H__
#define __HTTP_SERVER_H__

#include "types.h"
#include <netinet/in.h>

/**
 * @brief Server errors
 */
enum http_server_error {
    hse_bad_socket,
    hse_cant_bind,
    hse_cant_listen
};

/**
 * @brief HTTP server structure
 */
struct http_server {
    i32 socket;
    u32 port;
    struct sockaddr_in address;
    u32 addr_len;
};

/**
 * @brief Default server port
 */
static const u32 http_server__default_port = 8080;

/**
 * @brief Init HTTP server
 */
enum http_server_error http_server__init(struct http_server* server);

/**
 * @brief Main server func/thread
 */
void http_server__serve(struct http_server* server);


#endif // __HTTP_SERVER_H__
