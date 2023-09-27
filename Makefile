CC = gcc
CFLAGS = -Wall -Wextra -g

SRC_DIR = ./src
BIN_DIR = ./bin
UTILS_DIR = $(SRC_DIR)/utils

all: hack_assembler

hack_assembler: $(SRC_DIR)/main.c
	$(CC) $(CFLAGS) $^ -o $(BIN_DIR)/main -I ./src/utils

clean:
	rm -f ./bin/*