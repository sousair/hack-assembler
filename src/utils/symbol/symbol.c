#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <symbol.h>

int hash_name(char *name)
{
  int hash = 0;
  while (*name)
  {
    hash += *name;
    name++;
  }
  return hash % SYMBOL_HASH_TABLE_MAX_SIZE;
}

SYMBOL *new_symbol(char *name, __uint16_t address)
{
  SYMBOL *symbol = malloc(sizeof(SYMBOL));
  symbol->name = name;
  symbol->address = address;
  symbol->next = NULL;
  return symbol;
}

SYMBOL *add_symbol(char *name, __uint16_t address, SYMBOL *hash_table[SYMBOL_HASH_TABLE_MAX_SIZE])
{
  int index = hash_name(name);

  SYMBOL *e_symbol = hash_table[index];

  if (e_symbol == NULL)
  {
    SYMBOL *symbol = new_symbol(name, address);
    hash_table[index] = symbol;
    return symbol;
  }

  while(e_symbol->next != NULL)
  {
    e_symbol = e_symbol->next;
  }

  SYMBOL *n_symbol = new_symbol(name, address);
  
  e_symbol->next = n_symbol;
  return n_symbol;
}

SYMBOL *contains(char *name, SYMBOL *hash_table[SYMBOL_HASH_TABLE_MAX_SIZE])
{
  int index = hash_name(name);

  SYMBOL *symbol = hash_table[index];

  while (symbol != NULL)
  {
    if (strcmp(symbol->name, name) == 0)
    {
      return symbol;
    }
    symbol = symbol->next;
  }

  return NULL;
}
__uint16_t get_address(char *name, SYMBOL *hash_table[SYMBOL_HASH_TABLE_MAX_SIZE])
{
  SYMBOL *symbol = contains(name, hash_table);
  if (symbol == NULL)
  {
    return 0;
  }
  return symbol->address;
}
