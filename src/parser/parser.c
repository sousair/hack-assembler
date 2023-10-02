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
instruction get_instruction(char first_c);
void increment_symbol_address(int *address);

char *handle_a_instruction(char *var, int *address_count, SYMBOL hash_table[SYMBOL_HASH_TABLE_MAX_SIZE]);

char *handle_c_instruction(char *line);
char *get_c_dest(char *dest_values);
char *get_c_jump(char *jump_values);
char *get_c_comp(char *comp_values);
char *handle_one_char_comp(char first_comp_char);
char *handle_two_char_comp(char first_comp_char, char second_comp_char);
char *handle_three_char_comp(char first_comp_char, char second_comp_char, char last_comp_char);

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
      char *a_instruction = handle_a_instruction(var, &address_c, hash_table);
      free(binary_to_write);
      binary_to_write = a_instruction;
      write_to_file(hack_file, binary_to_write);
      break;
    }

    case L_INST:
    {
      continue;
    }

    case C_INST:
    {
      char *c_instruction = handle_c_instruction(clean_line);
      free(binary_to_write);
      binary_to_write = c_instruction;
      write_to_file(hack_file, binary_to_write);
      break;
    }
    }
  }
  free(binary_to_write);
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

void increment_symbol_address(int *address)
{
  int next_address_value = *address + 1;

  if (next_address_value > LAST_HACK_MEMORY_ADDRESS)
  {
    fprintf(stderr, "[Error] Max address exceeded\n");
    return;
  }

  if (next_address_value >= SCREEN_MEMORY_ADDRESS_START && next_address_value <= KEYBOARD_MEMORY_ADDRESS)
  {
    (*address) = KEYBOARD_MEMORY_ADDRESS + 1;
    return;
  }

  (*address)++;
}

bool is_number(const char *str)
{
  char *endptr;
  strtod(str, &endptr);

  return *endptr == '\0';
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

char *handle_a_instruction(char *var, int *address_count, SYMBOL hash_table[SYMBOL_HASH_TABLE_MAX_SIZE])
{
  if (is_number(var))
  {
    return parse_to_binary(atoi(var));
  }

  SYMBOL *find_symbol = contains(var, hash_table);

  if (find_symbol != NULL)
  {
    return parse_to_binary(find_symbol->address);
  }

  SYMBOL *new_symbol = add_symbol(var, *address_count, hash_table);
  increment_symbol_address(address_count);

  return parse_to_binary(new_symbol->address);
}

char *handle_c_instruction(char *line)
{
  char dest_values[4] = {'\0'};
  char comp_values[4] = {'\0'};
  char jump_values[4] = {'\0'};

  int line_size = strlen(line);

  char *value_to_write = comp_values;

  for (int char_index = 0; char_index < line_size; char_index++)
  {
    if (line[char_index] == '/')
    {
      break;
    }
    if (line[char_index] == '=')
    {
      strncpy(dest_values, comp_values, char_index);
      comp_values[0] = '\0';
      continue;
    }

    if (line[char_index] == ';')
    {
      value_to_write = jump_values;
      continue;
    }

    strncat(value_to_write, &line[char_index], 1);
  }

  char *dest = get_c_dest(dest_values);
  char *jump = get_c_jump(jump_values);
  char *comp = get_c_comp(comp_values);

  char a = strchr(comp_values, 'M') == NULL ? '0' : '1';

  char *c_instruction = malloc(sizeof(char) * (BINARY_SIZE + 1));

  sprintf(c_instruction, "111%c%s%s%s", a, comp, dest, jump);

  free(comp);
  free(dest);
  free(jump);

  return c_instruction;
}

char *get_c_dest(char *dest_values)
{
  char *dest = malloc(sizeof(char) * 4);
  strcpy(dest, "000\0");

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

  return dest;
}

char *get_c_jump(char *jump_values)
{
  char *jump = malloc(sizeof(char) * 4);
  jump[3] = '\0';

  if (jump_values[0] == '\0')
  {
    strcpy(jump, "000\0");
  }
  else if (strcmp(jump_values, "JGT") == 0)
  {
    strcpy(jump, "001\0");
  }
  else if (strcmp(jump_values, "JEQ") == 0)
  {
    strcpy(jump, "010\0");
  }
  else if (strcmp(jump_values, "JGE") == 0)
  {
    strcpy(jump, "011\0");
  }
  else if (strcmp(jump_values, "JLT") == 0)
  {
    strcpy(jump, "100\0");
  }
  else if (strcmp(jump_values, "JNE") == 0)
  {
    strcpy(jump, "101\0");
  }
  else if (strcmp(jump_values, "JLE") == 0)
  {
    strcpy(jump, "110\0");
  }
  else if (strcmp(jump_values, "JMP") == 0)
  {
    strcpy(jump, "111\0");
  }

  return jump;
}

char *get_c_comp(char *comp_values)
{
  char first_comp_c = comp_values[0];
  char mid_comp_c = comp_values[1];
  char last_comp_c = comp_values[2];

  if (mid_comp_c == '\0')
  {
    return handle_one_char_comp(first_comp_c);
  }
  else if (last_comp_c == '\0')
  {
    return handle_two_char_comp(first_comp_c, mid_comp_c);
  }
  else
  {
    return handle_three_char_comp(first_comp_c, mid_comp_c, last_comp_c);
  }

  return "000000\0";
}

char *handle_one_char_comp(char first_comp_char)
{
  char *comp = malloc(sizeof(char) * 7);
  comp[6] = '\0';
  if (isdigit(first_comp_char))
  {
    if (first_comp_char == '0')
    {
      strcpy(comp, "101010\0");
    }
    else
    {
      strcpy(comp, "111111\0");
    }
  }
  else if (first_comp_char == 'D')
  {
    strcpy(comp, "001100\0");
  }
  else
  {
    strcpy(comp, "110000\0");
  }
  return comp;
}

char *handle_two_char_comp(char first_comp_char, char second_comp_char)
{
  char *comp = malloc(sizeof(char) * 7);
  comp[6] = '\0';

  if (first_comp_char == '!')
  {
    if (second_comp_char == 'D')
    {
      strcpy(comp, "001101\0");
    }
    else
    {
      strcpy(comp, "110001\0");
    }
  }
  else if (first_comp_char == '-')
  {
    if (second_comp_char == '1')
    {
      strcpy(comp, "111010\0");
    }
    else if (second_comp_char == 'D')
    {
      strcpy(comp, "001111\0");
    }
    else
    {
      strcpy(comp, "110011\0");
    }
  }
  else if (first_comp_char == 'D')
  {
    if (second_comp_char == '1')
    {
      strcpy(comp, "011111\0");
    }
    else
    {
      strcpy(comp, "000111\0");
    }
  }
  else
  {
    strcpy(comp, "000000\0");
  }

  return comp;
}

char *handle_three_char_comp(char first_comp_char, char second_comp_char, char last_comp_char)
{
  char *comp = malloc(sizeof(char) * 7);
  comp[6] = '\0';

  if (second_comp_char == '&')
  {
    strcpy(comp, "000000\0");
  }
  else if (second_comp_char == '|')
  {
    strcpy(comp, "010101\0");
  }
  else if (second_comp_char == '+')
  {
    if (last_comp_char == '1')
    {
      if (first_comp_char == 'D')
      {
        strcpy(comp, "011111\0");
      }
      else
      {
        strcpy(comp, "110111\0");
      }
    }
    else
    {
      strcpy(comp, "000010\0");
    }
  }
  else if (second_comp_char == '-')
  {
    if (last_comp_char == '1')
    {
      if (first_comp_char == 'D')
      {
        strcpy(comp, "001110\0");
      }
      else
      {
        strcpy(comp, "110010\0");
      }
    }
    else
    {
      if (first_comp_char == 'D')
      {
        strcpy(comp, "010011\0");
      }
      else
      {
        strcpy(comp, "000111\0");
      }
    }
  }

  return comp;
}