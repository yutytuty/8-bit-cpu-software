C = gcc
CFLAGS = -Wall -Wextra -I.

BIN_DIR = bin
OBJ_DIR = obj
COMMON_DIR = common

EMU_DIR = emu
EMU_SRCS = $(wildcard $(EMU_DIR)/*.c)
EMU_OBJS = $(patsubst $(EMU_DIR)/%.c,$(OBJ_DIR)/%.o,$(EMU_SRCS))
EMU_EXEC := $(BIN_DIR)/emu

COMMON_HEADERS = $(wildcard $(COMMON_DIR)/*.h)

.PHONY: emu clean

emu: $(EMU_EXEC)

$(EMU_EXEC): $(EMU_OBJS) $(BIN_DIR)
	$(CC) $(CFLAGS) -o $@ $<

$(EMU_OBJS): $(EMU_SRCS) $(OBJ_DIR) $(COMMON_HEADERS)
	$(CC) $(CFLAGS) -c -o $@ $<

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)

