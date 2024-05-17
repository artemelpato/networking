#include <stdio.h>
#include <stdlib.h>

#define RED "\033[0;31m"
#define GREEN "\033[0;32m"
#define RESET_COLOR "\033[0m"
#define YELLOW "\033[0;33m"

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
