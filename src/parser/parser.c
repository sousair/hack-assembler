#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>

#include <symbol.h>
#include <remove-spaces.h>
#include <parser.h>

char *parse_to_binary(int decimal);
void write_to_file(FILE *hack_file, char *binary_to_write);
char *handle_c_instruction(char *line);
char *handle_l_instruction(char *var, SYMBOL hash_table[SYMBOL_HASH_TABLE_MAX_SIZE]);
char *handle_a_instruction(char *var, int *address_count, SYMBOL hash_table[SYMBOL_HASH_TABLE_MAX_SIZE]);
void get_comp(char comp_values[3], char comp[7]);
instruction get_instruction(char first_c);
bool is_number(const char *str);

void parse(FILE *assembly_file, SYMBOL hash_table[SYMBOL_HASH_TABLE_MAX_SIZE], FILE *hack_file)
{
  char line[256];
  int address_c = 16;
  char *binary_to_write = malloc(sizeof(char) * (BINARY_SIZE + 1));
  while (fgets(line, sizeof(line), assembly_file))
  {
    char *clean_line = remove_spaces(line);
    if (clean_line[0] == '/' || clean_line[0] == '\n' || clean_line[0] == '\0')
    {
      continue;
    }

    instruction inst_type = get_instruction(clean_line[0]);

    switch (inst_type)
    {
    case A_INST:
    {
      char *var = strtok(clean_line, "@");
      binary_to_write = handle_a_instruction(var, &address_c, hash_table);
      write_to_file(hack_file, binary_to_write);
      free(binary_to_write);
      break;
    }

    case L_INST:
    {
      continue;
    }

    case C_INST:
    {
      binary_to_write = handle_c_instruction(clean_line);
      write_to_file(hack_file, binary_to_write);
      free(binary_to_write);
      break;
    }
    }
  }
}

void write_to_file(FILE *hack_file, char *binary_to_write)
{
  fwrite(binary_to_write, sizeof(char), BINARY_SIZE, hack_file);
  fputc('\n', hack_file);
}

char *parse_to_binary(int decimal)
{
  char binary[BINARY_SIZE + 1] = "0000000000000000\0";
  
  int index = BINARY_SIZE - 1;
  while (decimal > 0)
  {
    binary[index] = (decimal % 2) + '0';
    decimal >>= 1;
    index--;
  }
  return strdup(binary);
}

char *handle_c_instruction(char *line)
{
  char dest_values[4] = {'\0'};
  char comp_values[4] = {'\0'};
  char jump_values[4] = {'\0'};

  int line_len = strlen(line);

  char *actual_v = comp_values;

  for (int i = 0; i < line_len; i++)
  {
    if (line[i] == '/')
    {
      break;
    }
    if (line[i] == '=')
    {
      strncpy(dest_values, comp_values, i);
      comp_values[0] = '\0';
      continue;
    }

    if (line[i] == ';')
    {
      actual_v = jump_values;
      continue;
    }

    strncat(actual_v, &line[i], 1);
  }

  char dest[4] = "000\0";
  if (dest_values[0] != '\0')
  {
    for (int i = 0; i < 3; i++)
    {
      if (dest_values[i] == '\0')
      {
        break;
      }
      switch (dest_values[i])
      {
      case 'A':
        dest[0] = '1';
        break;
      case 'D':
        dest[1] = '1';
        break;
      case 'M':
        dest[2] = '1';
        break;
      }
    }
  }

  char jump[4] = "000\0";
  if (jump_values[0] == '\0')
  {
    jump[0] = '0';
    jump[1] = '0';
    jump[2] = '0';
  }
  else if (strcmp(jump_values, "JGT") == 0)
  {
    jump[0] = '0';
    jump[1] = '0';
    jump[2] = '1';
  }
  else if (strcmp(jump_values, "JEQ") == 0)
  {
    jump[0] = '0';
    jump[1] = '1';
    jump[2] = '0';
  }
  else if (strcmp(jump_values, "JGE") == 0)
  {
    jump[0] = '0';
    jump[1] = '1';
    jump[2] = '1';
  }
  else if (strcmp(jump_values, "JLT") == 0)
  {
    jump[0] = '1';
    jump[1] = '0';
    jump[2] = '0';
  }
  else if (strcmp(jump_values, "JNE") == 0)
  {
    jump[0] = '1';
    jump[1] = '0';
    jump[2] = '1';
  }
  else if (strcmp(jump_values, "JLE") == 0)
  {
    jump[0] = '1';
    jump[1] = '1';
    jump[2] = '0';
  }
  else if (strcmp(jump_values, "JMP") == 0)
  {
    jump[0] = '1';
    jump[1] = '1';
    jump[2] = '1';
  }

  char comp[7] = "000000\0";

  get_comp(comp_values, comp);

  char a = '0';
  if (comp_values[0] == 'M' || comp_values[1] == 'M' || comp_values[2] == 'M')
  {
    a = '1';
  }

  char *c_instruction = malloc(sizeof(char) * (BINARY_SIZE + 1));
  sprintf(c_instruction, "111%c%s%s%s", a, comp, dest, jump);
  return c_instruction;
}

void get_comp(char comp_values[3], char comp[7])
{
  char first_comp_c = comp_values[0];
  char mid_comp_c = comp_values[1];
  char last_comp_c = comp_values[2];
  comp[6] = '\0';

  if (mid_comp_c == '\0')
  {
    if (isdigit(first_comp_c))
    {
      if (first_comp_c == '0')
      {
        comp = strcpy(comp, "101010\0");
        return;
      }
      comp = strcpy(comp, "111111\0");
      return;
    }
    else if (first_comp_c == 'D')
    {
      comp = strcpy(comp, "001100\0");
      return;
    }
    comp = strcpy(comp, "110000\0");
    return;
  }

  if (last_comp_c == '\0')
  {
    if (first_comp_c == '-')
    {
      if (mid_comp_c == '1')
      {
        comp = strcpy(comp, "111010\0");
        return;
      }
      else if (mid_comp_c == 'D')
      {
        comp = strcpy(comp, "001111\0");
        return;
      }
      comp = strcpy(comp, "110011\0");
      return;
    }
    if (first_comp_c == '!')
    {
      if (mid_comp_c == 'D')
      {
        comp = strcpy(comp, "001101\0");
        return;
      }
      comp = strcpy(comp, "110001\0");
      return;
    }
  }

  if (mid_comp_c == '&')
  {
    comp = strcpy(comp, "000000\0");
    return;
  }
  else if (mid_comp_c == '|')
  {
    comp = strcpy(comp, "010101\0");
    return;
  }
  else if (mid_comp_c == '+')
  {
    if (last_comp_c == '1')
    {
      if (first_comp_c == 'D')
      {
        comp = strcpy(comp, "011111\0");
        return;
      }
      comp = strcpy(comp, "110111\0");
      return;
    }
    comp = strcpy(comp, "000010\0");
  }
  else if (mid_comp_c == '-')
  {
    if (last_comp_c == '1')
    {
      if (first_comp_c == 'D')
      {
        comp = strcpy(comp, "001110\0");
        return;
      }
      comp = strcpy(comp, "110010\0");
      return;
    }

    if (first_comp_c == 'D')
    {
      comp = strcpy(comp, "010011\0");
      return;
    }
    comp = strcpy(comp, "000111\0");
    return;
  }
}

char *handle_l_instruction(char *var, SYMBOL hash_table[SYMBOL_HASH_TABLE_MAX_SIZE])
{
  SYMBOL *s = contains(var, hash_table);

  return parse_to_binary(s->address);
}

char *handle_a_instruction(char *var, int *address_count, SYMBOL hash_table[SYMBOL_HASH_TABLE_MAX_SIZE])
{
  if (is_number(var))
  {
    return parse_to_binary(atoi(var));
  }

  SYMBOL *find_s = contains(var, hash_table);

  if (find_s != NULL)
  {
    return parse_to_binary(find_s->address);
  }

  SYMBOL *s = add_symbol(var, (*address_count)++, hash_table);

  return parse_to_binary(s->address);
}

instruction get_instruction(char first_c)
{
  switch (first_c)
  {
  case '@':
    return A_INST;
  case '(':
    return L_INST;
  default:
    return C_INST;
  }
}

bool is_number(const char *str)
{
  char *endptr;
  strtod(str, &endptr);

  return *endptr == '\0';
}
