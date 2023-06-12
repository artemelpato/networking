#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "better_string.h"

#define PORT 8080

#define RED "\033[0;31m"
#define GREEN "\033[0;32m"
#define RESET_COLOR "\033[0m"
#define YELLOW "\033[0;33m"
#define INDEX "index.html"

void log_and_exit(const char*);
void log_info(const char*);
void check_and_exit(int cond, const char* msg);

typedef struct sockaddr SockAddr;
typedef struct sockaddr_in SockAddrIn;

int main() {
    int err = 0;

    int socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    check_and_exit(socket_fd, "can't create socket");
    log_info("succesfully opened socket");

    int opt = 1;
    err = setsockopt(socket_fd, SOL_SOCKET, 
                     SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt));
    check_and_exit(err, "can't set socket options");

    SockAddrIn address = {
        .sin_family = AF_INET,
        .sin_port = htons(PORT),
        .sin_addr.s_addr = INADDR_ANY
    };
    socklen_t addr_len = sizeof(address);

    err = bind(socket_fd, (SockAddr*)&address, sizeof(address));
    check_and_exit(err, "can't bind socket");
    log_info("succesfully bind socket");
   
    err = listen(socket_fd, 3);
    check_and_exit(err, "can't listen");

    char buffer[1024];

    while (1) {
        int client_socket = accept(socket_fd, (SockAddr*)&address, &addr_len);
        check_and_exit(client_socket, "can't accept");
        log_info("connection established");

        memset(buffer, 0, sizeof(buffer));
        int n_recv = recv(client_socket, buffer, sizeof(buffer) - 1, 0);
        (void) n_recv;

        /*char http_request[] = "GET / HTTP/1.1";*/
        /*if (strncmp(buffer, http_request, sizeof(http_request) - 1) != 0) {*/
            /*printf(GREEN "[MESSAGE RECEIVED]: " RESET_COLOR "%s", buffer);*/
            /*goto connection_close;*/
        /*}*/

        /*printf(GREEN "[MESSAGE RECEIVED]: " RESET_COLOR "%s", "got http request\n");*/

        StringView request = {
            .str = buffer, 
            .len = strlen(buffer)
        };

        StringView file_name = {0};

        StringView token = string_tokenize(&request, STR_VIEW_LITERAL("\n\r"));
        while (token.str) {

            StringView subtoken = string_tokenize(&token, STR_VIEW_LITERAL(" "));
            while (subtoken.str) {
                if (string_compare(subtoken, STR_VIEW_LITERAL("GET")) == 0) {
                    printf("GET TOKEN\n");
                    subtoken = string_tokenize(&token, STR_VIEW_LITERAL(" "));
                    subtoken.str++;
                    subtoken.len--;

                    file_name = subtoken;
                }

                subtoken = string_tokenize(&token, STR_VIEW_LITERAL(" "));
            }

            token = string_tokenize(&request, STR_VIEW_LITERAL("\n\r"));
        }
        
        if (!file_name.str || !file_name.len)
            file_name = STR_VIEW_LITERAL("index.html");
        printf("SHOULD SEND FILE %.*s\n", (int)file_name.len, file_name.str);

        char file_name_z[1024] = {0};
        snprintf(file_name_z, 1024, "%.*s", (int)file_name.len, file_name.str);

        FILE* index_file = fopen(file_name_z, "rb");
        if (!index_file) 
            goto connection_close;

        fseek(index_file, 0, SEEK_END);
        size_t n_bytes = ftell(index_file);
        fseek(index_file, 0, 0);

        StringView http_code = STR_VIEW_LITERAL("HTTP/1.0 200 OK\r\n\r\n");

        char* file_buffer = malloc(n_bytes * sizeof(char));
        /*read(fileno(index_file), file_buffer, n_bytes);*/
        fread(file_buffer, sizeof(char), n_bytes, index_file);

        char* response_buffer = malloc(sizeof(char) * (n_bytes + http_code.len));
        snprintf(
            response_buffer, sizeof(char) * (n_bytes + http_code.len),
            "%.*s%.*s", 
            (int)http_code.len, http_code.str, 
            (int)n_bytes, file_buffer);
                 
        write(client_socket, http_code.str, http_code.len);
        write(client_socket, file_buffer, n_bytes);
        /*write(client_socket, response_buffer, n_bytes + http_code.len);*/

        free(file_buffer);
        free(response_buffer);

        fclose(index_file);
connection_close:
        close(client_socket);
        log_info("connection closed");
    }

    shutdown(socket_fd, SHUT_RDWR);
    log_info("socket closed");

    return 0;
}

void log_and_exit(const char* msg) {
    printf(RED "[ERROR]:" RESET_COLOR " %s\n", msg);
    exit(-1);
}

void log_info(const char* msg) {
    printf(YELLOW "[INFO]:" RESET_COLOR " %s\n", msg);
    return;
}

void check_and_exit(int cond, const char* msg) {
    if (cond < 0) 
        log_and_exit(msg);
    return;
}

