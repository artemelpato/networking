#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORT 8080

void log_and_exit(const char*);
void log_info(const char*);

int main() {
    int err = 0;

    int socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_fd < 0) 
        log_and_exit("socket file desc is negative");
    log_info("succesfully opened socket");

    int opt = 1;
    err = setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,
                     &opt, sizeof(opt));
    if (err != 0) 
        log_and_exit("can't set socket options");

    struct sockaddr_in address;
    socklen_t addr_len = sizeof(address);
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    err = bind(socket_fd, (struct sockaddr*)&address, sizeof(address));
    if (err != 0)
        log_and_exit("can't bind socket");
    log_info("succesfully bind socket");
   
    char buffer[1024];
    memset(buffer, 0, 1024);
    err = listen(socket_fd, 3);
    if (err != 0) 
        log_info("can't listen");

    int new_socket = accept(socket_fd, (struct sockaddr*)&address, &addr_len);
    if (new_socket < 0) 
        log_info("can't accept");

    recv(new_socket, buffer, 1024, 0);
    printf("[MESSAGE RECEIVED]: %s", buffer);

    sleep(3);
    char hi[] = "oh hi!!!\n";
    send(new_socket, hi, sizeof(hi), 0);
    log_info("message sent");

    close(new_socket);
    shutdown(socket_fd, SHUT_RDWR);
    log_info("socket closed");
    return 0;
}

void log_and_exit(const char* msg) {
    printf("[ERROR]: %s\n", msg);
    exit(-1);
}

void log_info(const char* msg) {
    printf("[INFO]: %s\n", msg);
    return;
}

