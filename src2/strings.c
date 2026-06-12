/* strings.c
   implements B string functions
*/

#include "strings.h"

/* this fn should be called char() but that conflicts with c char type */
/* returns i'th char of string */
word chr(word* string, word i)
{
  word wrd;
  wrd = string[i/2];
  if(i & 1)
  {
    wrd &= 0x00ff;
  }
  else
  {
    wrd >>= 8;
  }
  return wrd;
}

/* lchar 
   the character chr is stored in the i-th character of string 
*/
void lchar(word* string, word i, word chr)
{
  word wrd = string[i/2];
  if(i & 1)
  {
    wrd &= 0xff00;
    wrd |= (chr & 0xff);
  }
  else
  {
    wrd &= 0x00ff;
    wrd |= (chr << 8);
  }
  string[i/2] = wrd;
}

/* bstrcmp
   not in B manual, but needed and borrowed from C
   Compares strings a,b 
   returns <0 if a < b, 0 if equal, >0 if a > b
*/
word bstrcmp(word* a, word* b)
{
  word rtn = 0;
  word i;
  word awrd,bwrd;
  i = 0;
  while(1)
  {
     awrd = a[i];
     bwrd = b[i];
     
  }
} 