#include <stdio.h>

#include "sim_common.h"

int main(int argc, char** argv)
{
  for(int i = 0; i < 256; i++)
  {
    printf("%03d: %s \n", i, RPN_strings[i]);
  }
  return 0;
}
