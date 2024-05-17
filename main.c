#include "types.h"
#include "http_server.h"
#include "stdio.h"

i32 main() {
    struct http_server server;
    enum http_server_error err = http_server__init(&server);
    if (err) {
        printf("exiting with error %d\n", err);
        return -1;
    }

    http_server__serve(&server);
    return 0;
}
