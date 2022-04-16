#include <stdint.h>
#include "definitions.h"


#define TOK_INT               256
#define TOK_IDENT             257
#define TOK_STRING            258
/* keywords */
#define TOK_AUTO              259
#define TOK_EXTRN             260
#define TOK_CASE              261
#define TOK_IF                262
#define TOK_ELSE              263
#define TOK_WHILE             264
#define TOK_SWITCH            265
#define TOK_GOTO              266
#define TOK_RETURN            267
/*  Add new keywords here */
/* do, static, etc */

#define TOK_INC               300  /* ++ */
#define TOK_DEC               301  /* -- */
#define TOK_EQ_EQ             302  /* == */
#define TOK_NOT_EQ            303  /* != */
#define TOK_LESS_EQ           304  /* <= */
#define TOK_GREAT_EQ          305  /* >= */
#define TOK_SHIFT_LEFT        306  /* << */
#define TOK_SHIFT_RIGHT       307  /* >> */
/* assignment operators */
#define TOK_EQ_OR             320  /* =|  */
#define TOK_EQ_AND            321  /* =&  */
#define TOK_EQ_EQ_EQ          322  /* === */
#define TOK_EQ_NOT_EQ         323  /* =!= */
#define TOK_EQ_LESS           324  /* =<  */
#define TOK_EQ_LESS_EQ        325  /* =<= */
#define TOK_EQ_GREAT          326  /* =>  */
#define TOK_EQ_GREAT_EQ       327  /* =>= */
#define TOK_EQ_SHIFT_LEFT     328  /* =<< */
#define TOK_EQ_SHIFT_RIGHT    329  /* =>> */
#define TOK_EQ_MINUS          330  /* =-  */
#define TOK_EQ_PLUS           331  /* =+  */
#define TOK_EQ_MOD            332  /* =%  */
#define TOK_EQ_STAR           333  /* =*  */
#define TOK_EQ_SLASH          334  /* =/  */
/* Add more operators here */
#define TOK_EOF               500 
#define TOK_UNDEFINED         999

word scanfile(char* fn);
word scan(void);
