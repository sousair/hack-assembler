#include <stdio.h>
#include <symbol.h>

#define BINARY_SIZE 16

enum Instructions
{
  A_INST = 'A',
  C_INST = 'C',
  L_INST = 'L'
};

#define instruction enum Instructions

void parse(FILE *assembly_file, SYMBOL hash_table[SYMBOL_HASH_TABLE_MAX_SIZE], FILE *hack_file);