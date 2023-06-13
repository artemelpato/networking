CC=clang
CFLAGS=-Wall -Wextra -Werror -g
LDFLAGS=

SRC= server.c better_string.c
OBJ=$(patsubst %.c, build/%.o, $(SRC))

.PHONY: all clean

all: server

server: $(OBJ) 
	$(CC) $^ -o $@ $(LDFLAGS)

build/%.o: %.c
	$(CC) $(CFLAGS) $< -c -o $@

clean: 
	rm -f server client

