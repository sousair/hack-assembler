CC = gcc
CFLAGS = -Wall -Wextra -g

SRC_DIR = ./src
BIN_DIR = ./bin
OBJ_DIR = ./obj

UTILS_DIR = $(SRC_DIR)/utils

SRC_FILES = $(wildcard $(SRC_DIR)/*.c) $(wildcard $(UTILS_DIR)/symbol/*.c)
OBJ_FILES = $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRC_FILES))

INC_DIRS = -I$(UTILS_DIR) -I$(UTILS_DIR)/symbol

all: hack_assembler

hack_assembler: $(OBJ_FILES)
	$(CC) $(CFLAGS) $^ -o $(BIN_DIR)/main

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) $(INC_DIRS) -c $< -o $@

clean:
	rm -rf $(BIN_DIR)/* $(OBJ_DIR)/*
