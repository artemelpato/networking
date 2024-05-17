CC=clang
CFLAGS=-Wall -Wextra -Werror -g
LDFLAGS=

SRC= server.c str_view.c
OBJ=$(patsubst %.c, build/%.o, $(SRC))

.PHONY: all clean

all: server

server: $(OBJ) 
	$(CC) $^ -o $@ $(LDFLAGS)

build/%.o: %.c
	$(CC) $(CFLAGS) $< -c -o $@

clean: 
	rm -f server client

