#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>

#include <symbol.h>
#include <parser.h>
#include <remove-spaces.h>

#define HACK_ASSEMBLY_EXTENSION ".asm"
#define HACK_ASSEMBLY_EXTENSION_SIZE 4

bool check_file_extension(int arg_count, char *arg_values[]);

void add_default_symbols(SYMBOL hash_table[SYMBOL_HASH_TABLE_MAX_SIZE]);
void read_label_symbols(FILE *assembly_file, SYMBOL hash_table[SYMBOL_HASH_TABLE_MAX_SIZE]);

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

  rewind(assembly_file);

  char *dot_position = strrchr(argv[1], '.');

  size_t file_name_length = dot_position - argv[1];

  char *output_file_name = malloc(file_name_length + strlen(".hack") + 1);
  
  memcpy(output_file_name, argv[1], file_name_length);
  output_file_name[file_name_length] = '\0';
  
  strcat(output_file_name, ".hack");

  FILE *hack_file = fopen(output_file_name, "wb");

  if (hack_file == NULL)
  {
    fprintf(stderr, "[Error] Could not create file %s\n", output_file_name);
    return 1;
  }

  parse(assembly_file, hash_table, hack_file);

  free(output_file_name);
  fclose(assembly_file);
  fclose(hack_file);
  return 0;
}

void read_label_symbols(FILE *assembly_file, SYMBOL hash_table[SYMBOL_HASH_TABLE_MAX_SIZE])
{
  int lines_count = 0;
  char line[256];

  while (fgets(line, sizeof(line), assembly_file))
  {
    char *clean_line = remove_spaces(line);
    char first_character = clean_line[0];

    if (first_character == '/' || first_character == '\n' || first_character == '\0')
    {
      continue;
    }

    if (first_character == '(')
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

      add_symbol(label_symbol, lines_count, hash_table);
      continue;
    }
    lines_count++;
  }
}

bool check_file_extension(int arg_count, char *arg_values[])
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