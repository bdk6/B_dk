/* ***********************************************************************
 * @file codegen.c
 * @copy Copyright 2022 William R Cooke
 * @brief Handles code generation for virtual B machine 
 * ******************************************************************** */

#include <stdio.h>
#include "definitions.h"
#include "scanner.h"
#include "codegen.h"
#include "interp.h"


/* ***********************************************************************
 * The dataBuffer holds literal data.  For an extrn definition, it will 
 * be the only buffer used.  During a function definition, there will 
 * be code and (local) literal data so both the code and data buffers 
 * will be used.  In-function literal data isn't named so the data 
 * values will be put into the code block.  References to this local 
 * data will be data-segment relative.  The relocation points will 
 * point to the code word that needs to be relocated, but the relocation 
 * will be data-segment relative instead of code-segment relative.
 * Every extrn definition gets it's own block.  Even a single variable.
 *   simpleVar;                   gets a bss block 
 *   vector[5] 2, 3, 5, 7, 9;     gets a data block 
 *   aFunction() { return(0); }   gets a code block 
 * A function definition can produce all three types of blocks:
 *   Code
 *   Data        auto localVec[] 1, 2, 3;
 *   BSS   ???   (Now? or when we extend to static locals?)
 * ******************************************************************** */

word codeBuffer[2048]; /* buffer for machine code */
word codeReloc[128];   /* list of code relative addresses to relocate */
word dataBuffer[2048]; /* Buffer for literal data. */
word dataReloc[128];   /* Relocation table for data */
word genCodeCount;     /* Code offset counter */
word genDataCount;     /* Data offset counter */
word genBSSCount;      /* BSS offset counter */


FILE* objfile;

/* ***********************************************************************
 * @fn writeWord
 * @brief Write a word to the obj file in Big Endian formal.
 * return 0 on success
 * ******************************************************************** */
word writeWord(word wrd)
{
  word rtn = 0;
  word status = 0;
  status = fputc((wrd >> 8) & 0xff, objfile);
  if(rtn == 0xffff)
  {
    /* ERROR end of file */
    rtn = status;
  }
  else
  {
    status = fputc((wrd & 0xff), objfile);
    if(rtn == 0xffff)
    {
      /* ERROR end of file */
      rtn = status;
    }
  }

  return rtn;
}



/* ***********************************************************************
 * @fn gen_initialize
 * @brief Get everything ready for code generation
 * return 0 on success
 * ******************************************************************** */
word gen_initialize(char* filename)
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

  objfile = fopen(filename, "w");
  if(objfile == NULL)
  {
    rtn = 1;
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
  rtn = fclose(objfile);


  return rtn;
}

/* ***********************************************************************
 * @fn gen_startCodeBlock
 * @brief Start building a code block.
 * return 
 * ******************************************************************** */
word gen_startCodeBlock()
{
  word rtn = 0;
  word i;
  genCodeCount = 0;
  genDataCount = 0;
  genBSSCount = 0;
  
  for(i = 0; i < 128; i++)
  {
    codeReloc[i] = 0;      /* Clear relocation bits */
    dataReloc[i] = 0;      
  }
  
  return rtn;
}

/* ***********************************************************************
 * @fn gen_addCodeWord
 * @brief Add a word to the current code block.
 * @param[in] d Word to add to code.
 * @param[in] reloc If non-zero, make this word relocatable.
 * return  Offset of this word.
 * ******************************************************************** */
word gen_addCodeWord(word d, word reloc)
{
  word rtn = 0;
  word rwrd;
  word rbit;
  rtn = genCodeCount;
  codeBuffer[genCodeCount] = d;
  if(reloc)
  {
    rwrd = genCodeCount >> 4;
    rbit = genCodeCount & 15;
    codeReloc[rwrd] |= (1 << rbit);  /* set reloc bit for this word */
  }
  genCodeCount++;

  return rtn;
}

/* ***********************************************************************
 * @fn gen_endCodeBlock
 * @brief Finalize and write the current code block
 * return 
 * ******************************************************************** */
word gen_endCodeBlock()
{
  word rtn = 0;
  // write header
  // write code
  // write reloc
  // write extrns

  return rtn;
}

/* ***********************************************************************
 * @fn gen_startDataBlock
 * @brief Start writing an initialized data block.
 * return Offset of this data item/block
 * ******************************************************************** */
/* ***********************************************************************
 * Data Block
 *   0  ID
 *   1  OFFSET
 *   2  NAME, 0
 *   N  WORDS
 *   N  DATA
 *   M  RELOC
 * ******************************************************************** */
word gen_startDataBlock(char* name)
{
  word rtn = 0;
  rtn = writeWord(OBJ_DATA);          /* Mark it as a data block */
  rtn |= writeWord(genDataCount);     /* Set the offset */
  /* TODO Name? */


  return rtn;
}

/* ***********************************************************************
 * @fn gen_addDataWord
 * @brief Add a word to the initialized data block
 * @param[in] d Data word to add.
 * @param[in] reloc If non-zero make this word relocatable.
 * return Offset in block of this item.
 * ******************************************************************** */
word gen_addDataWord(word d, word reloc)
{
  word rtn = 0;
  word rwrd;
  word rbit;
  rtn = genDataCount;
  dataBuffer[genDataCount] = d;
  if(reloc)
  {
    rwrd = genDataCount >> 4;
    rbit = genDataCount & 15;
    dataReloc[rwrd] |= (1 << rbit);  /* set reloc bit for this word */
  }
  genDataCount++;

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
 * @fn gen_addBSSWords
 * @brief Add a number of words (n) to the BSS block
 * @param[in] 
 * return Offset of this data item.
 * ******************************************************************** */
word gen_addBSSWord(word n)
{
  word rtn = 0;
  rtn = genBSSCount;
  genBSSCount += n;

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
 * @param[in] start_addr The starting IF address to update.
 * return Location of branch address
 * ******************************************************************** */
word gen_if_else(word start_addr)
{
  word rtn = 0;
  
  codeBuffer[genCodeCount++] = RPN_BR; /* Jump over the ELSE statement */
  rtn = genCodeCount;              /* This address needs to be updated */
  codeBuffer[genCodeCount++] = 0;  /* BRanch address to exit IF */
  /* update the IF jmp address */
  codeBuffer[start_addr] = genCodeCount; /* IF comes here on FALSE */

  /* TODO  Add address to relocation table */

  printf("\tBR\tIF_END\n");
  printf("IF_ELSE\n");

  return rtn;
}

/* ***********************************************************************
 * @fn gen_if_end
 * @brief Finalize IF statement
 * @param[in] previous_addr Address of previous BR to update.
 * return 0 on success
 * ******************************************************************** */
word gen_if_end(word previous_addr)
{
  word rtn = 0;

  codeBuffer[previous_addr] = genCodeCount;  /* make IF or ELSE jump here */
  printf("IF_END\n");

  return rtn;
}

/* ***********************************************************************
 * @fn gen_cond_start
 * @brief Begin a conditional statement
 * @param[in] 
 * return 0 on success
 * ******************************************************************** */
word gen_cond_start()
{
  word rtn = 0;

  return rtn;
}

/* ***********************************************************************
 * @fn gen_cond_true
 * @brief Finalize IF statement
 * @param[in] previous_addr Address of previous BR to update.
 * return 0 on success
 * ******************************************************************** */
word gen_cond_true()
{
  word rtn = 0;

  return rtn;
}

/* ***********************************************************************
 * @fn gen_cond_false
 * @brief Finalize IF statement
 * @param[in] previous_addr Address of previous BR to update.
 * return 0 on success
 * ******************************************************************** */
word gen_cond_false(word addr1)
{
  word rtn = 0;

  return rtn;
}

/* ***********************************************************************
 * @fn gen_cond_end
 * @brief Finalize IF statement
 * @param[in] previous_addr Address of previous BR to update.
 * return 0 on success
 * ******************************************************************** */
word gen_cond_end()
{
  word rtn = 0;

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

  // TODO pop parameters

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
 * @param[in] offset Offset of parameter from Parameter Pointer
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
 * @param[in] offset Offset of auto from Frame Pointer
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
 * @fn gen_binop
 * @brief Generate code for most binary operators.
 * @param[in] op The token for the desired operation.
 * return 
 * ******************************************************************** */
word gen_binop(word op)
{
  word rtn = 0;
  switch(op)
  {
  case '+':
    printf("\tADD\n");
    break;
  case '-':
    printf("\tSUB\n");
    break;
  case '*':
    printf("\tMUL\n");
    break;
  case '/':
    printf("\tDIV\n");
    break;
  case '%':
    printf("\tMOD\n");
    break;
  case '|':
    printf("\tOR\n");
    break;
  case '&':
    printf("\tAND\n");
    break;
  case '^':
    printf("\tXOR\n");
    break;

  case TOK_EQ_EQ:
    printf("\tCMPEQ\n");
    break;
  case TOK_NOT_EQ:
    printf("\tCMPNEQ\n");
    break;
  case '<':
    printf("\tCMPLESS\n");
    break;
  case '>':
    printf("\tCMPGRT\n");
    break;
  case TOK_LESS_EQ:
    printf("\tCMPLEQ\n");
    break;
  case TOK_GREAT_EQ:
    printf("\tCMPGEQ\n");
    break;

  case TOK_SHIFT_LEFT:
    printf("\tSHL\n");
    break;
  case TOK_SHIFT_RIGHT:
    printf("\tSHR\n");
    break;


  case '=':                    /* =   */
  case  TOK_EQ_OR:             /* =|  */
  case  TOK_EQ_AND:            /* =&  */
  case  TOK_EQ_EQ_EQ:          /* === */
  case  TOK_EQ_NOT_EQ:         /* =!= */
  case  TOK_EQ_LESS:           /* =<  */
  case  TOK_EQ_LESS_EQ:        /* =<= */
  case  TOK_EQ_GREAT:          /* =>  */
  case  TOK_EQ_GREAT_EQ:       /* =>= */
  case  TOK_EQ_SHIFT_LEFT:     /* =<< */
  case  TOK_EQ_SHIFT_RIGHT:    /* =>> */
  case  TOK_EQ_MINUS:          /* =-  */
  case  TOK_EQ_PLUS:           /* =+  */
  case  TOK_EQ_MOD:            /* =%  */
  case  TOK_EQ_STAR:           /* =*  */
  case  TOK_EQ_SLASH:          /* =/  */

  default:
    /* ERROR: illegal operator */
    break;
    

  }
  
  return rtn;
}

/* ***********************************************************************
 * @fn gen_unary
 * @brief Generate code for most unary operators.
 * @param[in] op The operator token to generate.
 * return 
 * ******************************************************************** */
word gen_unary(word op)
{
  word rtn = 0;

  switch(op)
  {
  case '-':
    printf("\tNEG\n");
    break;
  case '~':
    printf("\tCPL\n");
    break;
  case '!':
    printf("\tNOT\n");
    break;
  case '&':
    printf("\tTODO FIX ADDROF \n");
    /* TODO FIX  -- remove fetch */
    break;
  case '*':
    printf("\tFETCH\n");
    break;

  default:
    /* ERROR: illegal operator */
    break;

  }

  return rtn;
}


word gen_literal(word val)
{
  printf("\tPSHI\t%04X\n", val);

  return 0;
}







