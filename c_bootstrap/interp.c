/* ***********************************************************************
 * @file interp.c
 * @brief Implements interpreter for RPN virtual machine
 * ******************************************************************** */

#include <stdio.h>
#include "interp.h"

#define MEM_SIZE         65536


word memory[65536];      // System RAM
word PC;                 // Points to next instruction
word IR;                 // Holds current instruction
word SP;                 // Points to first empty spot
word ERROR;              // Holds error code if there is one

word dump()
{
  word rtn = 0;

  printf("PC:      %4X\n", PC);
  printf("SP:      %4X\n", SP);
  printf("IR:      %4X\n", IR);
  printf("ERROR:   %4X\n", ERROR);
  printf("TOS:     %4x\n", memory[SP +1]);

  return rtn;
}

word execute(void)
{
  word rtn = 1;     // should we keep running or stop

  switch(IR)
  {
  case RPN_NOP:
    break;
  case RPN_ADD:
    memory[SP + 2] = memory[SP + 2] + memory[SP + 1];
    SP++;
    break;
  case RPN_SUB:
    memory[SP+2] = memory[SP+2] - memory[SP + 1];
    SP++;
    break;
  case RPN_MUL:
    memory[SP+2] = memory[SP +2] * memory[SP + 1];
    SP++;
    break;
  case RPN_DIV:
    memory[SP + 2] = memory[SP + 2] / memory[SP + 1];
    SP++;
    break;
  case RPN_MOD:
    memory[SP + 2] = memory[SP + 2] % memory[SP + 1];
    SP++;
    break;
  case RPN_AND:
    memory[SP + 2] = memory[SP + 2] & memory[SP + 1];
    SP++;
    break;
  case RPN_OR:
    memory[SP + 2] = memory[SP + 2] | memory[SP + 1];
    SP++;
    break;
  case RPN_XOR:
    memory[SP + 2] = memory[SP + 2] ^ memory[SP + 1];
    SP++;
    break;

  case RPN_NOT:
    memory[SP + 1] = ~memory[SP + 1];
    break;
  case RPN_NEG:
    memory[SP + 1] = -memory[SP + 1];
    break;

  case RPN_BR:
    PC = memory[PC];
    break;
  case RPN_BRZ:
    SP++;
    if(memory[SP] == 0)
    {
      PC = memory[PC];
    }
    else
    {
      PC++;
    }
    break;
  case RPN_BRNZ:
    SP++;
    if(memory[SP] != 0)
    {
      PC = memory[PC];
    }
    else
    {
      PC++;
    }
    break;
  case RPN_FETCH:
    memory[SP + 1] = memory[memory[SP + 1]];
    break;
  case RPN_STORE:
    memory[memory[SP + 1]] = memory[SP + 2];
    SP += 2;
    break;
  case RPN_PSHI:
    memory[SP--] = memory[PC++];
    break;
  case RPN_POP:
    SP++;
    break;
  case RPN_DUP:
    memory[SP] = memory[SP + 1];
    SP--;
    break;

  case RPN_SETSP:
    SP = memory[PC++];
    break;

  case RPN_OUTCH:
    putchar(memory[SP + 1] & 0xff);
    SP++;
    break;
  case RPN_INCH:
    memory[SP] = getchar();
    SP--;
    break;
    
  case RPN_HALT:
    rtn = 0;
    break;
  default:
    rtn = 0;
    ERROR = 1;
    break;
  }

  return rtn;
}
  
    
word run(word start)
{
  word rtn = 0;
  word run = 1;     // run(1) or halt(0)
  PC = start;

  printf("Running...\n");
  while(run)
  {
    IR = memory[PC++];     // fetch instruction
    run = execute();       // execute instruction
  }
  dump();
  
  return rtn;
}

word clear(void)
{
  for(long i = 0; i < MEM_SIZE; i++)
  {
    memory[i] = 0;
  }
  PC = 0;
  IR = 0;
  SP = 0;
  ERROR = 0;

  return 0;
}

word loadBinary(word addr)
{
  word rtn = 0;


  return rtn;
}


int main(int argc, char** argv)
{
  int rtn = 0;
  printf("B_dk_interp Virtual Machine Interpreter \n");
  printf("For use with B_dk B compiler.\n");
  printf("Copyright 2022 Will Cooke \n");
  if(argc < 2)
  {
    printf("\n\nUsage: interp <filename> \n");
    printf("Where <filename> is the name of binary executable to run\n");
  }
  else
  {
    // load and run it
    printf("clear\n");
    clear();
    printf("Set halt\n");
    memory[998] = RPN_SETSP;
    memory[999] = 10000;
    memory[1000] = RPN_PSHI;
    memory[1001] = 'X';
    memory[1002] = RPN_OUTCH;
    memory[1003] = RPN_PSHI;
    memory[1004] = 'Y';
    memory[1005] = RPN_OUTCH;
    memory[1006] = RPN_HALT;
    memory[65535] = RPN_HALT;
    printf("run\n");
    run(0);
  }
  


  return rtn;
}
