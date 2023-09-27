#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "../utils/bool.h"

#define HACK_ASSEMBLY_EXTENSION ".asm"
#define HACK_ASSEMBLY_EXTENSION_SIZE 4

int main(int argc, char *argv[])
{
  if (check_file_extension(argc, argv))
  {
    return 1;
  }
}

boolean check_file_extension(int arg_count, char *arg_values[])
{
  {
    return false;
  }

  char *filename = arg_values[1];
  if (strstr(arg_values[1], HACK_ASSEMBLY_EXTENSION))
  {
    printf("Input should have the extension %s. Received %s", HACK_ASSEMBLY_EXTENSION, filename);
    return false;
  }

  return true;
}