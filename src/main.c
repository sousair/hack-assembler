#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#include <symbol.h>
#include <bool.h>

#define HACK_ASSEMBLY_EXTENSION ".asm"
#define HACK_ASSEMBLY_EXTENSION_SIZE 4

boolean check_file_extension(int arg_count, char *arg_values[]);
void increment_symbol_address(int *address);
void add_default_symbols(SYMBOL hash_table[SYMBOL_HASH_TABLE_MAX_SIZE]);
void read_label_symbols(FILE *assembly_file, SYMBOL hash_table[SYMBOL_HASH_TABLE_MAX_SIZE]);
char *remove_whitespaces(char line[]);

int main(int argc, char *argv[])
{
  if (!check_file_extension(argc, argv))
  {
    return 1;
  }

  FILE *assembly_file = fopen(argv[1], "r");

  if (assembly_file == NULL)
  {
    fprintf(stderr, "[Error] Could not open file %s\n", argv[1]);
    return 1;
  }

  SYMBOL hash_table[SYMBOL_HASH_TABLE_MAX_SIZE] = {NULL};

  add_default_symbols(hash_table);

  read_label_symbols(assembly_file, hash_table);

  return 0;
}

void read_label_symbols(FILE *assembly_file, SYMBOL hash_table[SYMBOL_HASH_TABLE_MAX_SIZE])
{
  int lines_count = 0;
  char line[256];

  while (fgets(line, sizeof(line), assembly_file))
  {
    char *clean_line = remove_whitespaces(line);
    char start_c = clean_line[0];

    if (start_c == '/' || start_c == '\n')
    {
      continue;
    }

    if (start_c == '(')
    {
      char *label_symbol = strtok(clean_line, "()");

      if (isdigit(label_symbol[0]))
      {
        fprintf(stderr, "[Error] Symbol %s should start with a letter\n", label_symbol);
        return;
      }

      if (contains(label_symbol, hash_table) != NULL)
      {
        fprintf(stderr, "[Error] Symbol %s already exists\n", label_symbol);
        return;
      }

      add_symbol(label_symbol, lines_count + 1, hash_table);
    }
    lines_count++;
  }
}

char *remove_whitespaces(char line[])
{
  int i = 0;
  int copy_i = 0;

  while (line[i])
  {
    if (!isblank(line[i]))
    {
      line[copy_i++] = line[i];
    }
    i++;
  }

  line[copy_i] = '\0';

  return strdup(line);
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
}