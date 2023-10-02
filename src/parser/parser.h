#include <stdio.h>
#include <symbol.h>

#define BINARY_SIZE 16
#define SCREEN_MEMORY_ADDRESS_START 0x4000
#define KEYBOARD_MEMORY_ADDRESS 0x6000
#define LAST_HACK_MEMORY_ADDRESS 0x7FFF

enum Instructions
{
  A_INST = 'A',
  C_INST = 'C',
  L_INST = 'L'
};

#define instruction enum Instructions

void parse(FILE *assembly_file, SYMBOL *hash_table[SYMBOL_HASH_TABLE_MAX_SIZE], FILE *hack_file);