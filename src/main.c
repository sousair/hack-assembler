#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <../utils/bool.h>

#define HACK_ASSEMBLY_EXTENSION ".asm"
#define HACK_ASSEMBLY_EXTENSION_SIZE 4
#define MAX_CHARACTERS_PER_LINE 512

boolean check_file_extension(int arg_count, char *arg_values[]);

int main(int argc, char *argv[])
{
  if (check_file_extension(argc, argv))
  {
    return -1;
  }
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