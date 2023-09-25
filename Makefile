CC = clang++
CFLAGS = -Wall -Wextra -I. -g -std=c++11

BIN_DIR = bin
OBJ_DIR = obj
COMMON_DIR = common

EMU_DIR = emu
EMU_SRCS = $(wildcard $(EMU_DIR)/*.cpp)
EMU_OBJS = $(patsubst $(EMU_DIR)/%.cpp,$(OBJ_DIR)/$(EMU_DIR)/%.o,$(EMU_SRCS))
EMU_EXEC := $(BIN_DIR)/emu

ASM_DIR = asm
ASM_SRCS = $(wildcard $(ASM_DIR)/*.cpp)
ASM_OBJS = $(patsubst $(ASM_DIR)/%.cpp,$(OBJ_DIR)/$(ASM_DIR)/%.o,$(ASM_SRCS))
ASM_EXEC := $(BIN_DIR)/asm

COMMON_SRCS = $(wildcard $(COMMON_DIR)/*.cpp)
COMMON_OBJS = $(patsubst $(COMMON_DIR)/%.cpp,$(OBJ_DIR)/$(COMMON_DIR)/%.o,$(COMMON_SRCS))

.PHONY: emu asm clean

all: emu asm

emu: $(EMU_EXEC)

asm: $(ASM_EXEC)

$(EMU_EXEC): $(EMU_OBJS) $(BIN_DIR) $(COMMON_OBJS)
	$(CC) $(CFLAGS) -o $@ $(EMU_OBJS) $(COMMON_OBJS)

$(ASM_EXEC): $(ASM_OBJS) $(BIN_DIR) $(COMMON_OBJS)
	$(CC) $(CFALGS) -o $@ $(ASM_OBJS) $(COMMON_OBJS)

$(OBJ_DIR)/$(EMU_DIR)/%.o: $(EMU_DIR)/%.cpp $(OBJ_DIR)/$(EMU_DIR) $(COMMON_OBJS)
	$(CC) $(CFLAGS) -c -o $@ $<

$(OBJ_DIR)/$(ASM_DIR)/%.o: $(ASM_DIR)/%.cpp $(OBJ_DIR)/$(ASM_DIR) $(COMMON_OBJS)
	$(CC) $(CFLAGS) -c -o $@ $<

$(OBJ_DIR)/$(COMMON_DIR)/%.o: $(COMMON_DIR)/%.cpp $(OBJ_DIR)/$(COMMON_DIR)
	$(CC) $(CFLAGS) -c -o $@ $<
obj/common/log.o: $(OBJ_DIR)/$(COMMON_DIR)
	$(CC) $(CFLAGS) -c -o $@ common/log.cpp

$(OBJ_DIR)/$(EMU_DIR):
	mkdir -p $@

$(OBJ_DIR)/$(ASM_DIR):
	mkdir -p $@

$(OBJ_DIR)/$(COMMON_DIR):
	mkdir -p $@

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)
