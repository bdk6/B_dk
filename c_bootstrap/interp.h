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
  RPN_ADD        = 1,   // Add top two stack entries
  RPN_SUB        = 2,   // Subtract top two stack entries
  RPN_MUL        = 3,   // Multiply top two stack entries
  RPN_DIV        = 4,   // Divide top two stack entries
  RPN_MOD        = 5,   // Divide, return remainder
  RPN_AND        = 6,   // AND top two stack entries
  RPN_OR         = 7,   // OR top two stack entries
  RPN_XOR        = 8,   // XOR top two stack entries

  RPN_CPL        = 16,  // One's complement top stack entry
  RPN_NEG        = 17,  // Two's complement top stack entry
  RPN_NOT        = 18,  // 1 if TOS = 0, 0 otherwise
  


  RPN_BR         = 32,  // Branch to following address
  RPN_BSR        = 33,  // Branch to Subroutine at addr
  RPN_BRZ        = 34,  // Branch if TOS is zero
  RPN_BRNZ       = 35,  // Branch if TOS is not zero
  RPN_RET        = 36,  // Return from subroutine
  RPN_CALL       = 37,  // B function call
  
  RPN_FETCH      = 48,  // Push value addressed by TOS
  RPN_STORE      = 49,  // Store value in NOS to address in TOS
  RPN_PSHI       = 50,  // Push immediate value onto stack
  RPN_POP        = 51,  // Pop TOS
  RPN_DUP        = 52,  // Duplicate value on TOS
  RPN_DROP       = 53,  // Remove top entry from stack

  RPN_SETSP      = 64,  // Set stack pointer
  RPN_GETSP      = 65,  // Get stack pointer onto stack
  RPN_SETPP      = 66,  // Set Parameter Pointer from stack
  RPN_GETPP      = 67,  // Get parameter pointer onto stack
  RPN_SETFP      = 68,  // Set Frame Pointer from stack
  RPN_GETFP      = 69,  // Get Frame Pointer onto stack
  RPN_SPI        = 70,  // Stack Pointer Immediate
  RPN_PPI        = 71,  // Parameter Pointer Immediate
  RPN_FPI        = 72,  // Frame Pointer Immediate
  

  RPN_OUTCH      = 250, // Write TOS as ASCII char to console
  RPN_INCH       = 251, // Get ASCII char from console to TOS
  RPN_TON        = 252, // Turn on tracing
  RPN_TOFF       = 253, // Turn off tracing

  RPN_HALT       = 255, // Halt execution


} RPN_OP_t;


extern char* RPN_strings[];
