#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <symbol.h>
#include <bool.h>

#define HACK_ASSEMBLY_EXTENSION ".asm"
#define HACK_ASSEMBLY_EXTENSION_SIZE 4

boolean check_file_extension(int arg_count, char *arg_values[]);
void increment_symbol_address(int *address);
void add_default_symbols(SYMBOL hash_table[SYMBOL_HASH_TABLE_MAX_SIZE]);

int main(int argc, char *argv[])
{
  if (!check_file_extension(argc, argv))
  {
    return 1;
  }

  SYMBOL hash_table[SYMBOL_HASH_TABLE_MAX_SIZE] = { NULL };

  add_default_symbols(hash_table);
  return 0;
}

boolean check_file_extension(int arg_count, char *arg_values[])
{
  if (arg_count != 2)
  {
    // TODO: Add proper usage message.
    fprintf(stderr, "[Error] Input file is mandatory\n");
    return false;
  }

  char *filename = arg_values[1];
  if (strstr(arg_values[1], HACK_ASSEMBLY_EXTENSION) == NULL)
  {
    fprintf(stderr, "[Error] Input should have the extension %s. Received %s\n", HACK_ASSEMBLY_EXTENSION, filename);
    return false;
  }

  return true;
}

void increment_symbol_address(int *address)
{
  int reserved_addresses[2] = {16384, 24576};

  (*address)++;

  if (*address == reserved_addresses[0] || *address == reserved_addresses[1])
  {
    (*address)++;
  }

  if (*address > 0x7FFF)
  {
    fprintf(stderr, "[Error] Max address exceeded\n");
    *address = -1;
  }
}

void add_default_symbols(SYMBOL hash_table[SYMBOL_HASH_TABLE_MAX_SIZE])
{
  add_symbol("R0", 0, hash_table);
  add_symbol("R1", 1, hash_table);
  add_symbol("R2", 2, hash_table);
  add_symbol("R3", 3, hash_table);
  add_symbol("R4", 4, hash_table);
  add_symbol("R5", 5, hash_table);
  add_symbol("R6", 6, hash_table);
  add_symbol("R7", 7, hash_table);
  add_symbol("R8", 8, hash_table);
  add_symbol("R9", 9, hash_table);
  add_symbol("R10", 10, hash_table);
  add_symbol("R11", 11, hash_table);
  add_symbol("R12", 12, hash_table);
  add_symbol("R13", 13, hash_table);
  add_symbol("R14", 14, hash_table);
  add_symbol("R15", 15, hash_table);
  add_symbol("SP", 0, hash_table);
  add_symbol("LCL", 1, hash_table);
  add_symbol("ARG", 2, hash_table);
  add_symbol("THIS", 3, hash_table);
  add_symbol("THAT", 4, hash_table);
  add_symbol("SCREEN", 16384, hash_table);
  add_symbol("KBD", 24576, hash_table);
  add_symbol("LOOP", 4, hash_table);
}