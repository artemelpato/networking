#include "http_server.h"
#include <sys/socket.h>
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "str_view.h"
#include "unistd.h"
#include "logger.h"
#include "http_request.h"


enum http_server_error http_server__init(struct http_server* server) {
    i32 err = 0;
    i32 opt = 1;

    server->port = HTTP_SERVER_DEFAULT_PORT;

    server->address = (struct sockaddr_in){
        .sin_family      = AF_INET,
        .sin_port        = htons(server->port),
        .sin_addr.s_addr = INADDR_ANY
    };
    server->addr_len = sizeof(server->address);
    
    server->socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server->socket < 0)
        return hse_bad_socket;
    log_info("socket()");

    err = setsockopt(server->socket, SOL_SOCKET,
                     SO_REUSEADDR | SO_REUSEPORT, 
                     &opt, sizeof(opt));

    err = bind(server->socket, (struct sockaddr*)&server->address, sizeof(server->address));
    if (err)
        return hse_cant_bind;
    log_info("bind()");

    err = listen(server->socket, 3);
    if (err)
        return hse_cant_listen;
    log_info("listen()");

    return 0;
}

/**
 * @brief Accept connection
 */
static inline i32 http_server__accept(struct http_server* server) {
    return accept(server->socket, (struct sockaddr*)&server->address, &server->addr_len);
}

/**
 * @brief Handle GET HTTP method
 */
static void handle_get_method(struct http_server* server, struct http_request* req) {
    char requested_filepath[1024] = {0};
    FILE* file;
    const char success_code[] = "HTTP/1.0 200 OK\r\n\r\n";
    u32 bytes_to_send;
    char* response_buffer;
    u32 response_buffer_size;

    if (!server | !req) {
        return;
    }

    printf("Requested file: " STR_VIEW_F "\n",
           STR_VIEW_F_ARG(req->requested_file));

    snprintf(requested_filepath, sizeof(requested_filepath), 
             "%.*s", (i32)req->requested_file.len, req->requested_file.str);

    file = fopen(requested_filepath, "rb");
    if (!file) {
        return;
    }

    fseek(file, 0, SEEK_END);
    bytes_to_send = ftell(file);
    fseek(file, 0, 0);

    response_buffer_size = bytes_to_send;
    response_buffer = malloc(response_buffer_size + 1);
    if (!response_buffer) {
        return;
    }
    memset(response_buffer, 0, response_buffer_size + 1);

    fread(response_buffer, sizeof(char), bytes_to_send, file);

    write(server->socket, success_code, sizeof(success_code));
    write(server->socket, response_buffer, response_buffer_size);

    free(response_buffer);
}

void http_server__serve(struct http_server* server) {
    while (1) {
        i32 client;
        struct http_request req;

        log_info("started accepting");
        client = http_server__accept(server);
        if (client < 0)
            check_and_exit(client, "can't accept");
        log_info("connection established");

        http_request__read(&req, client);
        switch (req.method) {
        case hrm_get:
            handle_get_method(server, &req);
            break;

        default:
            break;
        }

        close(client);
        log_info("connection closed");
    }

    shutdown(server->socket, SHUT_RDWR);
}
