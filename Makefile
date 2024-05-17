CC=clang
CFLAGS=-Wall -Wextra -Werror -g
LDFLAGS=
OUT_DIR=build
EXE=server

SRCS=$(wildcard *.c)
HEADERS=$(wildcard *.h)
OBJS=$(patsubst %.c, $(OUT_DIR)/%.o, $(SRCS))

.PHONY: all clean

all: $(OUT_DIR)/$(EXE)

$(OUT_DIR)/$(EXE): $(OBJS) 
	$(CC) $^ -o $@ $(LDFLAGS)

$(OUT_DIR)/%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) $< -c -o $@

clean: 
	rm -f build/*

