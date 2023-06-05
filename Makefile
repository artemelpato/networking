CC=clang
CFLAGS=-Wall -Wextra -Werror

.PHONY: all clean

all: server client

%: %.c
	$(CC) $(CFLAGS) $< -o $@

clean: 
	rm -f server client

