/* ***********************************************************************
 * @file interp.h
 * @brief Header for RPN interpreter
 * ******************************************************************** */

#include "definitions.h"

/* ***********************************************************************
 * Machine description:
 * PC = Program Counter
 * PP = Parameter Pointer -> points to passed in parms on stack.
 * FP = Frame Pointer -> Points to local variables on stack.
 * SP = Stack Pointer
 * ... more

 * ******************************************************************** */

/* ***********************************************************************
 * RPN opcodes
 * ADD      Add top two stack entries
 * SUB      Subtract top two stack entries
 * MUL      Multiply top two stack entries
 * DIV      Divide top two stack entries
 * MOD      Divide and return remainder
 * AND      AND top two stack entries
 * OR       OR top two stack entries
 * ******************************************************************** */

typedef enum RPN_OP
{
  RPN_NOP        = 0,   // No operatiorn
  RPN_ADD,              // Add top two stack entries
  RPN_SUB,              // Subtract top two stack entries
  RPN_MUL,              // Multiply top two stack entries
  RPN_DIV,              // Divide top two stack entries
  RPN_MOD,              // Divide, return remainder
  RPN_AND,              // AND top two stack entries
  RPN_OR,               // OR top two stack entries
  RPN_XOR,              // XOR top two stack entries

  RPN_CPL,              // One's complement top stack entry
  RPN_NEG,              // Two's complement top stack entry
  


  RPN_BR,               // Branch to following address
  RPN_BSR,              // Branch to Subroutine at addr
  RPN_BRZ,              // Branch if TOS is zero
  RPN_BRNZ,             // Branch if TOS is not zero
  RPN_FETCH,            // Push value addressed by TOS
  RPN_STORE,            // Store value in NOS to address in TOS
  RPN_PSHI,             // Push immediate value onto stack
  RPN_POP,              // Pop TOS
  RPN_DUP,              // Duplicate value on TOS

  RPN_SETSP,            // Set stack pointer

  RPN_OUTCH,            // Write TOS as ASCII char to console
  RPN_INCH,             // Get ASCII char from console to TOS

  RPN_HALT,             // Halt execution


} RPN_OP_t;

