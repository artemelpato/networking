CC=tcc
CFLAGS=-Wall -Wextra -Werror -g
LDFLAGS=
OUT_DIR=build
EXE=server

SRCS=$(wildcard *.c)
HEADERS=$(wildcard *.h)
OBJS=$(patsubst %.c, $(OUT_DIR)/%.o, $(SRCS))

.PHONY: all clean

all: $(OUT_DIR)/$(EXE)
	$(info Done linking executable "$(EXE)")

$(OUT_DIR)/$(EXE): $(OBJS) 
	$(info Linking executable...)
	@$(CC) $^ -o $@ $(LDFLAGS)

$(OUT_DIR)/%.o: %.c $(HEADERS)
	$(info Building object "$@")
	@$(CC) $(CFLAGS) $< -c -o $@

clean: 
	$(info Cleaning build directory)
	@rm -f build/*

