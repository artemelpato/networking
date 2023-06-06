#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

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

    SockAddrIn address;
    socklen_t addr_len = sizeof(address);
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    err = bind(socket_fd, (SockAddr*)&address, sizeof(address));
    check_and_exit(err, "can't bind socket");
    log_info("succesfully bind socket");
   
    err = listen(socket_fd, 3);
    check_and_exit(err, "can't listen");

    int client_socket = accept(socket_fd, (SockAddr*)&address, &addr_len);
    check_and_exit(client_socket, "can't accept");
    log_info("connection established");

    char buffer[1024];
    memset(buffer, 0, sizeof(buffer));
    int n_recv = recv(client_socket, buffer, sizeof(buffer) - 1, 0);
    (void) n_recv;
    printf(GREEN "[MESSAGE RECEIVED]: " RESET_COLOR "%s", buffer);

    char index_page[1 << 15];
    FILE* index_file = fopen(INDEX, "r");
    read(fileno(index_file), index_page, sizeof(index_page));

    memset(buffer, 0, sizeof(buffer));
    snprintf(buffer, sizeof(buffer), "HTTP/1.0 200 OK\r\n\r\n%s", index_page);
    write(client_socket, buffer, strlen(buffer));

    /*while (n_recv > 0) {*/
        /*if (strncmp(buffer, "end\n", 4) == 0) {*/
            /*log_info("got end");*/
            /*break;*/
        /*}*/

        /*printf(GREEN "[MESSAGE RECEIVED]: " RESET_COLOR "%s", buffer);*/
        /*memset(buffer, 0, 1024);*/
        /*n_recv = recv(client_socket, buffer, sizeof(buffer) - 1, 0);*/
    /*}*/

    close(client_socket);
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

