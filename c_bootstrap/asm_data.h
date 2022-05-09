

#ifndef _ASM_DATA_H_
#define _ASM_DATA_H_

#include "definitions.h"

extern char* opcodeStrings[];
extern word opcodeSizes[];
extern word opcodeValues[];

#define TOK_IDENT       256
#define TOK_NUM         257
#define TOK_STRING      258
#define TOK_SHL         259
#define TOK_SHR         260
#define TOK_LESS_EQ     261
#define TOK_GREAT_EQ    262
#define TOK_EQ_EQ       263
#define TOK_NOT_EQ      264
#define TOK_UNDEFINED   65534
#define TOK_NONE        65535

#endif

