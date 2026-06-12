#include <stdio.h>
#include <stdint.h>
#include <ctype.h>

uint16_t fnvprime = 253;
uint16_t offset = 99; // arbitrary non-zero

uint16_t hist[65536];

char str[256];

uint16_t hash(char* s)
{
  uint16_t hash = offset;
  int i = 0;
  do
  {
    char c = str[i++];
    if ( ! isalpha(c)) break;
    hash *= fnvprime;
    hash &= (uint16_t) c;
  }  while(i < 255);
  hist[hash]++;
  return hash;
}

  

int main(void)
{
  FILE* words = fopen("/usr/share/dict/words", "r");
  if (words == NULL) return -1;
  printf("entering read loop\n");
  while( fgets(str, 256, words) != NULL)
  {
    putchar('.');
    printf("hash: %d___%s", hash(str), str);
  }
  printf("done with while loop\n");
  fclose(words);
  int min = 65535;
  int max = 0;
  for(int i = 0; i < 65536; i++)
  {
    if(hist[i] > max ) max = hist[i];
    if(hist[i] < min ) min = hist[i];
  }
  printf("MIN: %d MAX: %d\n", min, max);
  return 0;
}

