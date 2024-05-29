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

    server->port = http_server__default_port;

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

    i32 opt = 1;
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

void http_server__serve(struct http_server* server) {
    /*char buffer[1024];*/

    while (1) {
        log_info("started accepting");
        i32 client = http_server__accept(server);
        if (client < 0)
            check_and_exit(client, "can't accept");
        log_info("connection established");

        struct http_request req;
        http_request__read(&req, client);

        printf("SHOULD SEND FILE %.*s\n", (i32)req.requested_file.len, req.requested_file.str);

        char file_name_z[1024] = {0};
        snprintf(file_name_z, 1024, "%.*s", (i32)req.requested_file.len, req.requested_file.str);

        FILE* index_file = fopen(file_name_z, "rb");
        if (!index_file) 
            goto connection_close;

        fseek(index_file, 0, SEEK_END);
        size_t n_bytes = ftell(index_file);
        fseek(index_file, 0, 0);

        struct str_view http_code = STR_VIEW_LITERAL("HTTP/1.0 200 OK\r\n\r\n");

        char* file_buffer = malloc(n_bytes * sizeof(char));
        fread(file_buffer, sizeof(char), n_bytes, index_file);

        char* response_buffer = malloc(sizeof(char) * (n_bytes + http_code.len));
        snprintf(
            response_buffer, sizeof(char) * (n_bytes + http_code.len),
            "%.*s%.*s", 
            (i32)http_code.len, http_code.str, 
            (i32)n_bytes, file_buffer);
                 
        write(client, http_code.str, http_code.len);
        write(client, file_buffer, n_bytes);
        /*write(client_socket, response_buffer, n_bytes + http_code.len);*/

        free(file_buffer);
        free(response_buffer);

        fclose(index_file);
connection_close:
        close(client);
        log_info("connection closed");
    }

    shutdown(server->socket, SHUT_RDWR);
}
