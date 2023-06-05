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

    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) 
        log_and_exit("socket file desc is negative");
    log_info("opened socket");

    struct sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_port = htons(PORT);
    inet_pton(AF_INET, "127.0.0.1", &address.sin_addr);
    
    err = connect(sock, (struct sockaddr*)&address, sizeof(address));
    if (err != 0) 
        log_and_exit("can't connect");
    log_info("succesfully connected");

    char message[] = "Hello!!!!!!!\n";
    send(sock, message, sizeof(message), 0);
    log_info("message sent");

    char buffer[1024];
    memset(buffer, 0, 1024);
    recv(sock, buffer, sizeof(buffer), 0);
    printf("[MESSAGE RECEIVED]: %s", buffer);

    close(sock);
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
