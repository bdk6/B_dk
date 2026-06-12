/* bi.c                                                                    */
/* copyright 2024 William R Cooke                                          */
/* b interpreter                                                           */
/* runs program of "bytecodes" produced by bc, the b compiler              */

#include "definitions.h"
#include <stdio.h>
#include <stdint.h>

word mem[65536];

enum opcodes
{
  OP_NOP = 0,
  OP_PHC = 1,
  OP_FCH = 2,
  OP_SWP = 3,
  OP_NEG = 4,
  OP_NOT = 5,
  OP_MUL = 6,
  OP_DIV = 7,
  OP_MOD = 8,
  OP_ADD = 9,
  OP_SUB = 10,
  OP_SHL = 11,
  OP_SHR = 12,
  OP_LSS = 13,
  OP_GRT = 14,
  OP_LEQ = 15,
  OP_GEQ = 16,
  OP_AND = 17,
  OP_XOR = 18,
  OP_OR  = 19,
  OP_JMP = 20,
  OP_JZ  = 21,
  OP_SRV = 22,
  OP_RTN = 23,
  OP_SFP = 24,
  OP_ILL = 127
};

word PC;
word SP;
word FP;

void init_mem(void)
{
  for(int i = 0; i < 65536; i++)
  {
    mem[i] = 127; // illegal opcode
  }
}

int loadFile(char* filename)
{
  int rtn = 0;

  return rtn;
}

int run(void)
{
  int rtn = 0;
  word temp;

  do
  {
    /* code */
    word op = mem[PC++];
    switch(op)
    {
      case OP_NOP:
        break;

      case OP_PHC:
        mem[--SP] = mem[PC++];
        break;

      case OP_FCH:
        // TODO
        printf("FCH not implemented!\n");
      case OP_SWP:
        temp = mem[SP];
        mem[SP] = mem[SP+1];
        mem[SP+1] = temp;
        break;

      case OP_NEG:
        mem[SP] = -mem[SP];
        break;

      case OP_NOT:
        mem[SP] = ~mem[SP];
        break;

      case OP_MUL:
        mem[SP+1] *= mem[SP];
        SP++;
        break;

      case OP_DIV:
        // TODO add zero check
        mem[SP+1] = mem[SP] / mem[SP+1];
        SP++;
        break;

      case OP_MOD:
        // TODO add zero check
        mem[SP+1] = mem[SP] % mem[SP+1];
        SP++;
        break;

      case OP_ADD:
        mem[SP+1] += mem[SP];
        SP++;
        break;

      case OP_SUB:
        mem[SP+1] = mem[SP] - mem[SP+1];
        SP++;
        break;

      case OP_SHL:
        // TODO check signed/unsigned behavior
        mem[SP] <<= 1;
        break;

      case OP_SHR:
        // TODO check signed/unsigned behavior
        mem[SP] >>= 1;
        break;

      case OP_LSS:
        
      case OP_GRT:
      case OP_LEQ:
      case OP_GEQ:
      case OP_AND:
      case OP_XOR:
      case OP_OR:
      case OP_JMP:
      case OP_JZ:
      case OP_SRV:
      case OP_RTN:
      case OP_SFP:

      case OP_ILL:
      default:
        break;
    }
  } while (1);
  


  return rtn;
}

int main(int argc, char* argv[])
{
  int rtn = 0;
  if(argc < 2)
  {
    printf("Usage:  bi progname \n");
    return -1;
  }
  printf("Running the B program: %s \n", argv[1]);
  PC = 0;
  SP = 0;
  FP = 0;
  init_mem();




  return rtn;
}