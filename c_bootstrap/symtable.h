#include <stdio.h>
#include <stdint.h>
#include "definitions.h"


#define ENTRIES 511


#define FLAG_AUTO        ( 1 << 0 )   /* Auto variable */
#define FLAG_EXTRN       ( 1 << 1 )   /* Extrn variable */
#define FLAG_PARAMETER   ( 1 << 2 )   /* Function parameter */
#define FLAG_FUNCTION    ( 1 << 3 )   /* Function (implies extrn) */
#define FLAG_VEC         ( 1 << 4 )   /* Variable is a vector */

//extern word stringtable[16000];
extern char stringtable[16000];

extern word stringpointer;

word sym_initialize(void);
word sym_find(char* name);
word sym_isThere(word index);
word sym_insert(char* name, word flags, word value);
word sym_setFlags(word index, word flags);
word sym_setValue(word index, word val);
word sym_getFlags(word index);
word sym_getValue(word index);
word sym_dumpTable(void);

