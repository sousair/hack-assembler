#ifndef SYMBOL_H
#define SYMBOL_H

#include <stdint.h>

#define SYMBOL_HASH_TABLE_MAX_SIZE 50
typedef struct SYMBOL_T
{
  char *name;
  __uint16_t address;
  struct SYMBOL_T *next;
} SYMBOL;

SYMBOL *add_symbol(char *name, __uint16_t address, SYMBOL *hash_table[SYMBOL_HASH_TABLE_MAX_SIZE]);
SYMBOL *contains(char *name, SYMBOL *hash_table[SYMBOL_HASH_TABLE_MAX_SIZE]);
__uint16_t get_address(char *name, SYMBOL *hash_table[SYMBOL_HASH_TABLE_MAX_SIZE]);
void free_symbol_table(SYMBOL *hash_table[SYMBOL_HASH_TABLE_MAX_SIZE]);

#endif
