#include "definitions.h"


extern word gen_align(void);
extern word gen_pushConstant(word con);
extern word gen_string(word str);
extern word gen_fetch(void);
extern word gen_swap(void);
extern word gen_neg(void);
extern word gen_not(void);
extern word gen_mul(void);
extern word gen_div(void);
extern word gen_mod(void);
extern word gen_add(void);
extern word gen_sub(void);
extern word gen_shl(void);
extern word gen_shr(void);
extern word gen_less(void);
extern word gen_grtr(void);
extern word gen_lesseq(void);
extern word gen_grteq(void);
extern word gen_and(void);
extern word gen_xor(void);
extern word gen_or(void);
extern word gen_jmp(word addr);
extern word gen_jz(word addr);
extern word gen_srv(void);    /* save return val */
extern word gen_rtn(void);
extern word gen_sfp(void);    /* save frame ptr */
extern word gen_auto(word c);   /* create auto var */
extern word gen_jsk(void);    /* jump to stack */
extern word gen_drp(void);    /* drop */
extern word gen_equal(void);
extern word gen_noteq(void);
extern word gen_dup(void);




