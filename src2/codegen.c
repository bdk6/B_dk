/* codegen.c */
/* copyright 2024 William R Cooke */
/* generate code for B */

#include <stdio.h>

#include <stdint.h>

typedef int16_t word;
typedef uint8_t byte;

word OP_NOP = 0;  /* NOP */
word OP_PHC = 1;  /* PusH Constant */
word OP_FCH = 2;  /*   Fetch?  */
word OP_SWP = 3;  /* SWaP */
word OP_NEG = 4;  /* NEGate */
word OP_NOT = 5;  /* NOT           */
word OP_MUL = 6;  /* MULtiply      */
word OP_DIV = 7;  /* DIVide        */
word OP_MOD = 8;  /* MODulo        */
word OP_ADD = 9;  /* ADD           */
word OP_SUB = 10;  /* SUBtract      */
word OP_SHL = 11;  /* SHift Left    */
word OP_SHR = 12;  /* SHift Right   */
word OP_LSS = 13;  /* LeSS than     */
word OP_GRT = 14;  /* GReaTer than  */
word OP_LEQ = 15;  /* Less EQual    */
word OP_GEQ = 16;  /* Greater EQual */
word OP_AND = 17;  /* AND           */
word OP_XOR = 18;  /* XOR           */
word OP_OR  = 19;  /* OR            */
word OP_JMP = 20;  /* JuMP to addr  */
word OP_JZ  = 21;  /* Jump on Zero  */
word OP_SRV = 22;  /* Save Return Value */
word OP_RTN = 23;  /* RETVAL -> retval, TOS -> FP, TOS->PC*/
word OP_SFP = 24;  /* Save Frame Pointer: FP->stack, FP=SP*/
word OP_ILL = 127; /* Illegal opcode */


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
extern word gen_auto(word c);




word PC = 0;
byte mem[65536];

word gen_byte(word b)
{
  word rtn = PC;
  mem[PC++] = (b & 0xff);

  return rtn;
}

word gen_word(word w)
{
  word rtn = PC;
  gen_byte( (w>>8) & 0xff ); /* big endian */
  gen_byte( w & 0xff );

  return rtn;
}

word gen_align(void)
{
  word rtn = 0;
  if(PC & 0x01) { printf("%04x %02x NOP\n", PC, OP_NOP); gen_byte(OP_NOP); }

  return rtn;
}

word gen_pushConstant(word con)
{
  word rtn = PC;
  printf("%04x %02x PHC %04x\n", PC, OP_PHC, con);
  gen_byte(OP_PHC);
  gen_word(con);

  return rtn;
}

word gen_string(word str)
{
  word addr = 0;
  gen_align();
  addr = PC;
  printf("%04x string\n", PC);
  // TODO copy string words

  return addr;
}

word gen_fetch(void)
{
  word rtn = 0;
  printf("%04x %02x FCH\n", PC, OP_FCH);
  gen_byte(OP_FCH);

  return rtn;
}

word gen_swap(void)
{
  word rtn = 0;
  printf("%04x %02x SWP\n", PC, OP_SWP);
  gen_byte(OP_SWP);
  return rtn;
}

word gen_neg(void)
{
  word rtn = 0;
  printf("%04x %02x NEG\n", PC, OP_NEG);
  gen_byte(OP_NEG);
  return rtn;
}

word gen_not(void)
{
  word rtn = 0;
  printf("%04x %02x NOT\n", PC, OP_NOT);
  gen_byte(OP_NOT);
  return rtn;
}

word gen_mul(void)
{
  word rtn = 0;
  printf("%04x %02x MUL\n", PC, OP_MUL);
  gen_byte(OP_MUL);
  return rtn;
}

word gen_div(void)
{
  word rtn = 0;
  printf("%04x %02x DIV\n", PC, OP_DIV);
  gen_byte(OP_DIV);
  return rtn;
}

word gen_mod(void)
{
  word rtn = 0;
  printf("%04x %02x MOD\n", PC, OP_MOD);
  gen_byte(OP_MOD);
  return rtn;
}

word gen_add(void)
{
  word rtn = 0;
  printf("%04x %02x SUB\n", PC, OP_ADD);
  gen_byte(OP_ADD);
  return rtn;
}

word gen_sub(void)
{
  word rtn = 0;
  printf("%04x %02x SUB\n", PC, OP_SUB);
  return rtn;
}

word gen_shl(void)
{
  word rtn = 0;
  printf("%04x %02x SHL \n", PC, OP_SHL);
  return rtn;
}

word gen_shr(void)
{
  word rtn = 0;
  printf("%04x %02x SHR\n", PC, OP_SHR);
  return rtn;
}

word gen_less(void)
{
  word rtn = 0;
  printf("%04x %02x LSS\n", PC, OP_LSS);
  return rtn;
}
word gen_grtr(void)
{
  word rtn = 0;
  printf("%04x %02x GRT\n", PC, OP_GRT);
  return rtn;
}
word gen_lesseq(void)
{
  word rtn = 0;
  printf("%04x %02x LEQ\n", PC, OP_LEQ);
  return rtn;
}
word gen_grteq(void)
{
  word rtn = 0;
  printf("%04x %02x GEQ\n", PC, OP_GEQ);
  return rtn;
}

word gen_and(void)
{
  word rtn = 0;
  printf("%04x %02x AND\n", PC, OP_AND);
  return rtn;
}

word gen_xor(void)
{
  word rtn = 0;
  printf("%04x %02x XOR\n", PC, OP_XOR);
  return rtn;
}

word gen_or(void)
{
  word rtn = 0;
  printf("%04x %02x OR\n", PC, OP_OR);
  return rtn;
}

word gen_jmp(word addr)
{
  word rtn = 0;
  printf("%04x %02x JMP %04x\n", PC, OP_JMP, addr);
  return rtn;
}

word gen_jz(word addr)
{
  word rtn = 0;
  printf("%04x %02x JZ  %04x\n", PC, OP_JZ, addr);
  return rtn;
}

word gen_save_return_value(void)
{
  word rtn = 0;
  printf("%04x %02x SAVE_RETURN_VALUE\n", PC, OP_SRV );
  return rtn;
}

word gen_rtn(void)
{
  word rtn = 0;
  printf("%04x %02x RTN\n", PC, OP_RTN);
  return rtn;
}

word gen_svp(void) /* save frame ptr*/
{
  word rtn = 0;
  printf("%04x %02x SFP -- Save Frame Pointer\n", PC, OP_SFP);
  return rtn;
}

word gen_auto(word c) /* */
{
  word rtn = 0;
  printf("%04x %02x generate auto \n", PC, 0);
  return rtn;
}

/* jump to address on top of stack */
word gen_jsk(void)
{
  word rtn = 0;
  printf("%04x %02x JSK -- jump to stack\n", PC, 0);
  return rtn;
}

/* drop */
word gen_drp(void)
{
  word rtn = 0;
  printf("%04x %02x DRP -- drop\n", PC, 0);
  return rtn;
}

word gen_equal(void)
{
  word rtn = 0;
  printf("%04x %02x EQL\n", PC, 0);
  return rtn;
}

word gen_noteq(void)
{
  word rtn = 0;
  printf("%04x %02x NEQ\n", PC, 0);
  return rtn;
}

word gen_dup(void)
{
  word rtn = 0;
  printf("%04x %02x DUP\n", PC, 0);
  return rtn;
}

extern word gen_sfp(void)    /* save frame ptr */
{
  word rtn = 0;
  printf("%04x %02x SFP\n", PC, OP_SFP);
  return rtn;
}


