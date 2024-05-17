#include "http_request.h"
#include "sys/socket.h"
#include "stdio.h"

#define BUFSIZE 128

/**
 * @brief Get HTTP method from token
 */
static enum http_request_method http_request__get_method(struct str_view token) {
    if (str_view__compare(token, STR_VIEW_LITERAL("GET")) == 0)
        return hrm_get;
    return hrm_unsupported;
}

i32 http_request__read(struct http_request* req, i32 fd) {
    if (!req || fd < 0)
        return -1;

    req->method = hrm_unsupported;
    req->requested_file = STR_VIEW_LITERAL("");

    char buffer[BUFSIZE] = {0};
    recv(fd, buffer, sizeof(buffer) - 1, 0);

    struct str_view buffer_view = {
        .str = buffer,
        .len = BUFSIZE
    };

    // just method parsing for now
    struct str_view line = str_view__tokenize(&buffer_view, STR_VIEW_LITERAL("\n\r"));
    struct str_view method_token = str_view__tokenize(&line, STR_VIEW_LITERAL(" "));
    req->method = http_request__get_method(method_token);
    req->requested_file = str_view__tokenize(&line, STR_VIEW_LITERAL(" "));

    if (!req->requested_file.str || !req->requested_file.len)
        req->requested_file = STR_VIEW_LITERAL("index.html");

    printf("Waiting for file %*.s\n", (i32)req->requested_file.len, req->requested_file.str);

    return 0;
}
