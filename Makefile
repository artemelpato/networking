CC=clang
CFLAGS=-Wall -Wextra -Werror

.PHONY: all clean

all: server
	
server: server.c
	$(CC) $(CFLAGS) $< -o $@

clean: 
	rm -f server

