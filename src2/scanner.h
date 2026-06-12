#include "definitions.h"

/* The single character tokens simply have   */
/* their ascii value.  Below are the values  */
/* for all multi-character token types.      */
extern word TOK_IDENT;
extern word TOK_INT;
extern word TOK_STRING;
extern word TOK_COMMENT;
extern word TOK_AUTO;
extern word TOK_CASE;
extern word TOK_DEFAULT;
extern word TOK_ELSE;
extern word TOK_EXTRN;
extern word TOK_GOTO;
extern word TOK_IF;
extern word TOK_RETURN;
extern word TOK_SWITCH;
extern word TOK_WHILE;

extern word TOK_LESSEQ;   /* <= */
extern word TOK_SHLFT;   /* << */
extern word TOK_GRTEQ;   /* >= */
extern word TOK_SHRT;   /* >> */
extern word TOK_NOTEQ;   /* != */
extern word TOK_EQEQ;   /* == */
extern word TOK_EQPLUS;   /* =+ */
extern word TOK_EQMIN;   /* =- */
extern word TOK_EQSTAR;   /* =* */
extern word TOK_EQSLSH;   /* =/ */
extern word TOK_EQMOD;   /* =% */
extern word TOK_EQSHL;   /* =<< */
extern word TOK_EQSHR;   /* =>> */
extern word TOK_EQAND;   /* =& */
extern word TOK_EQOR;   /* =| */
extern word TOK_EQXOR;   /* =^ */
extern word TOK_PLPL;   /* ++ */
extern word TOK_MINMIN;   /* -- */

extern word TOK_COMMENT;   /* comments */        

extern word TOK_EOF;
extern word TOK_UNDEFINED;

extern word line_num;
extern word scan(void);