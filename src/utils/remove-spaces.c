#include <ctype.h>
#include <string.h>

#include <remove-spaces.h>

char *remove_spaces(char line[])
{
  int i = 0;
  int copy_i = 0;

  while (line[i])
  {
    if (!isspace(line[i]))
    {
      line[copy_i++] = line[i];
    }
    i++;
  }

  line[copy_i] = '\0';

  return strdup(line);
}