CC = clang++
CFLAGS = -Wall -Wextra -I. -g -std=c++11

BIN_DIR = bin
OBJ_DIR = obj
COMMON_DIR = common

EMU_DIR = emu
EMU_SRCS = $(wildcard $(EMU_DIR)/*.cpp)
EMU_OBJS = $(patsubst $(EMU_DIR)/%.c,$(OBJ_DIR)/%.o,$(EMU_SRCS))
EMU_EXEC := $(BIN_DIR)/emu

ASM_DIR = asm
ASM_SRCS = $(wildcard $(ASM_DIR)/*.cpp)
ASM_OBJS = $(patsubst $(ASM_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(ASM_SRCS))
ASM_EXEC := $(BIN_DIR)/asm

COMMON_HEADERS = $(wildcard $(COMMON_DIR)/*.h)

.PHONY: emu asm clean

all: emu # asm

emu: $(EMU_EXEC)

$(EMU_EXEC): $(EMU_OBJS) $(BIN_DIR)
	$(CC) $(CFLAGS) -o $@ $(EMU_OBJS)

$(ASM_EXEC): $(ASM_OBJS) $(BIN_DIR)
	$(CC) $(CFALGS) -o $@ $(ASM_OBJS)

$(OBJ_DIR)/%.o: $(EMU_DIR)/%.cpp $(OBJ_DIR) $(COMMON_HEADERS)
	$(CC) $(CFLAGS) -c -o $@ $<

$(OBJ_DIR)/%.o: $(ASM_DIR)/%.cpp $(OBJ_DIR) $(COMMON_HEADERS)
	$(CC) $(CFLAGS) -c -o $@ $<

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)
