/* ***********************************************************************
 * @file codegen.c
 * @copy Copyright 2022 William R Cooke
 * @brief Handles code generation for virtual B machine 
 * ******************************************************************** */

#include <stdio.h>
#include "definitions.h"
#include "interp.h"

word genDataCount;
word genBSSCount;
word genCodeCount;

word codeBuffer[2048];

/* ***********************************************************************
 * @fn gen_initialize
 * @brief Get everything ready for code generation
 * return 0 on success
 * ******************************************************************** */
word gen_initialize()
{
  word rtn = 0;
  word counter = 0;
  
  genDataCount = 0;
  genBSSCount = 0;
  genCodeCount = 0;

  while(counter < 2048)
  {
    codeBuffer[counter] = 0;
    counter++;
  }

  return rtn;
}

/* ***********************************************************************
 * @fn gen_finish
 * @brief Finalize code generation
 * return 0 on success
 * ******************************************************************** */
word gen_finish()
{
  word rtn = 0;


  return rtn;
}


/* ***********************************************************************
 * @fn gen_startDataBlock
 * @brief Start writing an initialized data block.
 * return 0 on success
 * ******************************************************************** */
word gen_startDataBlock()
{
  word rtn = 0;


  return rtn;
}

/* ***********************************************************************
 * @fn gen_addDataWord
 * @brief Add a word to the initialized data block
 * @param[in] d Data word to add.
 * return Offset in block of this item.
 * ******************************************************************** */
word gen_addDataWord(word d)
{
  word rtn = 0;


  return rtn;
}

/* ***********************************************************************
 * @fn gen_endDataBlock
 * @brief Finalize writing this block of data.
 * return 
 * ******************************************************************** */
word gen_endDataBlock()
{
  word rtn = 0;


  return rtn;
}

/* ***********************************************************************
 * @fn gen_startBSSClock
 * @brief Start writing an unitialized data block.
 * return 
 * ******************************************************************** */
word gen_startBSSBlock()
{
  word rtn = 0;


  return rtn;
}

/* ***********************************************************************
 * @fn gen_addBSSWord
 * @brief Add a word to the unitialized data block.
 * return Offset of this data item.
 * ******************************************************************** */
word gen_addBSSWord(word d)
{
  word rtn = 0;

  return rtn;
}

/* ***********************************************************************
 * @fn gen_endBSSBlock
 * @brief Finalize writing this block of data.
 * return 0 on success
 * ******************************************************************** */
word gen_endBSSBlock()
{
  word rtn;

  return rtn;
}

/* ***********************************************************************
 * @fn gen_if_start
 * @brief Generate beginning of IF statement
 * return Location of branch address.
 * ******************************************************************** */
word gen_if_start()
{
  word rtn;

  codeBuffer[genCodeCount++] = RPN_BRZ;
  rtn = genCodeCount;               /* return the location to update */
  codeBuffer[genCodeCount++] = 0;   /* branch address */
  /* TODO Add address to relocation table */

  printf("\tBRZ\tIF_ELSE\n");
  
  return rtn;
}

/* ***********************************************************************
 * @fn gen_if_else
 * @brief Generate ELSE branch for IF statement.
 * return Location of branch address
 * ******************************************************************** */
word gen_if_else()
{
  word rtn = 0;
  rtn = genCodeCount;
  codeBuffer[genCodeCount] = 98; // code for jmp
  genCodeCount++;
  /* TODO  Add address to relocation table */

  printf("\tBR\tIF_END\n");
  printf("IF_ELSE\n");

  return rtn;
}

/* ***********************************************************************
 * @fn gen_if_end
 * @brief Finalize IF statement
 * return 0 on success
 * ******************************************************************** */
word gen_if_end()
{
  word rtn = 0;
  printf("IF_END\n");

  return rtn;
}

/* ***********************************************************************
 * @fn gen_while_start
 * @brief Generate beginning of WHILE statement.
 * return Offset location to jump back to.
 * ******************************************************************** */
word gen_while_start()
{
  word rtn = 0;
  printf("WH_ST\n");  

  rtn = genCodeCount; /* where to jump back to */

  return rtn;
}

/* ***********************************************************************
 * @fn gen_while_exp
 * @brief Generate beginning of WHILE statement.
 * return Offset location to jump back to.
 * ******************************************************************** */
word gen_while_exp()
{
  word rtn = 0;
  printf("\tBRZ\tWH_END\n");
  
  codeBuffer[genCodeCount++] = RPN_BRZ;
  rtn = genCodeCount; /* location of address to update at end */
  codeBuffer[genCodeCount++] = 0;

  return rtn;
}

/* ***********************************************************************
 * @fn gen_while_end
 * @brief Code to branch back to start and location to exit.
 * return 
 * ******************************************************************** */
word gen_while_end(word start_add, word exp_addr)
{
  word rtn = 0;
  printf("\tBR\tWH_ST\n");
  printf("WH_END\n");

  codeBuffer[genCodeCount++] = RPN_BR;
  codeBuffer[genCodeCount++] = start_add;
  /* counter points to next instruction */
  codeBuffer[exp_addr] = genCodeCount;  /* update branch to end */

  return rtn;
}

/* ***********************************************************************
 * @fn gen_function_call
 * @brief Generate a (static) function call.
 * return 
 * ******************************************************************** */
word gen_function_call(word addr)
{
  word rtn = 0;
  printf("\tBSR\t%04X\n", addr);
  // TODO write reloc

  return rtn;
}

/* ***********************************************************************
 * @fn gen_function_return
 * @brief Generate a return from a function.
 * return 
 * ******************************************************************** */
word gen_function_return()
{
  word rtn = 0;
  printf("\tRTN\n");

  return rtn;
}

/* ***********************************************************************
 * @fn gen_goto
 * @brief Generate a GOTO statement.
 * @param[in] addr The address to jump to.
 * return 
 * ******************************************************************** */
word gen_goto(word addr)
{
  word rtn = 0;
  printf("\tBR\t%04X\n", addr);
  // TODO write reloc

  return rtn;
}

/* ***********************************************************************
 * @fn gen_get_parameter
 * @brief Generate code to move a parameter address to TOS.
 * return 
 * ******************************************************************** */
word gen_get_parameter(word offset)
{
  word rtn = 0;
  printf("\tOFFSPP\t%04X\n", offset); // OFFset Parameter Pointer

  return rtn;
}

/* ***********************************************************************
 * @fn gen_get_auto
 * @brief Generate code to move an auto var address to TOS.
 * return 
 * ******************************************************************** */
word gen_get_auto(word offset)
{
  word rtn = 0;
  printf("\tOFFSFP\t%04X\n", offset); // OFFset Frame Pointer

  return rtn;
}

/* ***********************************************************************
 * @fn gen_get_extrn
 * @brief Generate code to move an extrn address to TOS.
 * return 
 * ******************************************************************** */
word gen_get_extrn(word addr)
{
  word rtn = 0;
  printf("\tPSHI\t%04X\n", addr);

  return rtn;
}

/* ***********************************************************************
 * @fn gen_add
 * @brief Generate code to add two numbers.
 * return 
 * ******************************************************************** */
word gen_add()
{
  word rtn = 0;
  printf("\tADD\n");

  return rtn;
}

/* ***********************************************************************
 * @fn gen_subtract
 * @brief Generate code to subtract two numbers.
 * return 
 * ******************************************************************** */
word gen_subtract()
{
  word rtn = 0;
  printf("\tSUB\n");

  return rtn;
}

/* ***********************************************************************
 * @fn gen_multiply
 * @brief Generate code to multiply two numbers.
 * return 
 * ******************************************************************** */
word gen_multiply()
{
  word rtn = 0;
  printf("\tMUL\n");

  return rtn;
}

/* ***********************************************************************
 * @fn gen_divide
 * @brief Generate code to divide two numbers.
 * return 
 * ******************************************************************** */
word gen_divide()
{
  word rtn = 0;
  printf("\tDIV\n");

  return rtn;
}

/* ***********************************************************************
 * @fn gen_modulo
 * @brief Generate code to take one number modulo another.
 * return 
 * ******************************************************************** */
word gen_modulo()
{
  word rtn = 0;

  return rtn;
}

/* ***********************************************************************
 * @fn gen_and
 * @brief Generate code to AND two numbers.
 * return 
 * ******************************************************************** */
word gen_and()
{
  word rtn = 0;
  printf("\tAND\n");

  return rtn;
}

/* ***********************************************************************
 * @fn gen_or
 * @brief Generate code to OR two numbers.
 * return 
 * ******************************************************************** */
word gen_or()
{
  word rtn = 0;
  printf("\tOR\n");

  return rtn;
}

/* ***********************************************************************
 * @fn gen_xor
 * @brief Generate code to XOR two numbers.
 * return 
 * ******************************************************************** */
word gen_xor()
{
  word rtn = 0;
  printf("\tXOR\n");

  return rtn;
}

/* ***********************************************************************
 * @fn gen_negate
 * @brief Generate code to take negative of a number.
 * return 
 * ******************************************************************** */
word gen_negate()
{
  word rtn = 0;
  printf("\tNEG\n");

  return rtn;
}

/* ***********************************************************************
 * @fn gen_complement
 * @brief Generate code to take (one's) complement of a number.
 * return
 * ******************************************************************** */
word gen_complement()
{
  word rtn = 0;
  printf("\tCPL\n");

  return rtn;
}







