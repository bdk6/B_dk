/* strings.h
   header for B string functions
*/

#include "definitions.h"

/* this fn should be called char() but that conflicts with c char type */
/* returns i'th char of string */
word chr(word* string, word i);


/* lchar 
   the character chr is stored in the i-th character of string 
*/
void lchar(word* string, word i, word chr);
