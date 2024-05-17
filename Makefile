CC=clang
CFLAGS=-Wall -Wextra -Werror -g
LDFLAGS=

SRC= http_server.c str_view.c main.c
OBJ=$(patsubst %.c, build/%.o, $(SRC))

.PHONY: all clean

all: main

main: $(OBJ) 
	$(CC) $^ -o $@ $(LDFLAGS)

build/%.o: %.c
	$(CC) $(CFLAGS) $< -c -o $@

clean: 
	rm -f main build/*

