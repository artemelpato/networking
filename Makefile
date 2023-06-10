CC=clang
CFLAGS=-Wall -Wextra -Werror

.PHONY: all clean

all: server client better_string

%: %.c
	$(CC) $(CFLAGS) $< -o $@

clean: 
	rm -f server client

