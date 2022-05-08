/* ***********************************************************************
 * @file asm_sim.c
 * @Copy Copyright 2022 William R Cooke
 * @brief The simple assembler for the simulator 
 * ******************************************************************** */
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdint.h>
#include "definitions.h"
#include "asm_data.h"


static char lineBuffer[128];
static word nextIndex;
word operationIndex;
word operandIndex;

FILE* infile;
FILE* outfile;

word pass;
word symbols[256];
char strings[8192];
word nextSym = 0;
word nextString = 1;
word codeCounter;
word dataCounter;
word bssCounter;

// new


word initialize(char* filename);
word readLine(void);
word pass1(void);
word pass2(void);
word finalize(void);
word getLabel(void);
word getOperation(void);
word getOperand(void);
word symFind(char* name);
word symInsert(word idx, char* name, word value);
word findOpcode(char* str);

int main(int argc, char** argv)
{
  int rtn = 0;
  word result;
  printf("Starting assembly.\n");
  if(initialize(argv[1]))
    {
      printf("Exiting...\n");
      return(1);
    }
  //while(readLine() != 65535);
  printf("Starting pass 1 \n");
  result = pass1();
  printf("Result is %d \n", result);
  if(!result)
  {
    printf("Starting pass 2\n");
    //result = pass2();
  }
  rtn = result;
  printf("Pass 2 result is %d \n", result);
  printf("Finalizing...\n");
  finalize();
  printf("Done.\n");

  return rtn;
}

word initialize(char* filename)
{
  word rtn = 0;
  word len;
  word idx;

  if(filename != NULL)
  {
    strcpy(lineBuffer, filename); //outFileName, filename);
    len = strlen(filename);
    for(idx = len; idx > 0; idx--)
    {
      if(lineBuffer[idx] == '.') //outFileName[idx] == '.')
      {
        lineBuffer[idx] = 0; //outFileName[idx] = 0;
        break;
      }
    }
    strcat(lineBuffer, ".bin"); //outFileName, ".bin");
    infile = fopen(filename, "r");
    outfile = fopen(lineBuffer, "w"); //outFileName, "w");

    for(idx = 0; idx < 256; idx++)
    {
      symbols[idx] = 0;
    }
    for(idx = 0; idx < 8192; idx++)
    {
      strings[idx] = 0;
    }
    nextSym = 0;
    nextString = 1; /* 0 index means empty */

    if(infile == NULL || outfile == NULL)
    {
      printf("Could not open input file\n");
      rtn = 1;
    }
    if(outfile == NULL)
    {
      printf("Could not open output file\n");
      rtn = 1;
    }
  }
  else
  {
    printf("No file name specified\n");
    rtn = 1;
  }

  return rtn;
}

/* ***********************************************************************
 * @fn readLine
 * @brief Reads a line of code from input file
 * @return Number of chars read or 65535 on EOF 
 * ******************************************************************** */
word readLine(void)
{
  word rtn;
  word idx;
  int ch;
  idx = 0;
  operationIndex = 0;
  operandIndex = 0;
  
  while( (ch = getc(infile)) != EOF)
  {
    if(ch == '\r')
    {
      ch = 0;
    }
    if(ch == '\n')
    {
      ch = 0;
      lineBuffer[idx] = 0;
      break;
    }
    else
    {
      if(idx < 255)
      {
        lineBuffer[idx] = ch;
      }
      idx++;
    }
  }
  if(ch == EOF)
  {
    rtn = 65535;
    lineBuffer[idx] = 0;
  }
  else
  {
    rtn = idx;
  }
  
  printf("%s\n", lineBuffer);
  nextIndex = 0;   // where to scan next token
  return rtn;
}

/* ***********************************************************************
 * @fn pass1
 * @brief Performs first pass (of two) of the assembly.
 * @return 0 if successful, non-zero on errors.
 * ******************************************************************** */
word pass1(void)
{
  word rtn = 0;
  word result = 0;
  word labelLength;
  word labelValue;
  word operationStart;
  word opcodeIndex;
  word opcodeValue;
  word opcodeSize;
  word symidx;
  
  codeCounter = 0;
  dataCounter = 0;
  bssCounter = 0;
  pass = 1;

  do
  {
    result = readLine();
    labelLength = getLabel();  /* zero if no label */
    //printf("Label len: %d\n", labelLength);
    operationStart = getOperation();
    //printf("Operation Start: %d\n", operationStart);
    labelValue = codeCounter; // tentative
    if(operationStart != 0)
    {
      if(strcmp(&lineBuffer[operationStart], "EQU") == 0)
      {
        // set labelValue to exp
      }
      else if(strcmp(&lineBuffer[operationStart], "SET") == 0)
      {
        // set labelValue to exp
      }
      else if(strcmp(&lineBuffer[operationStart], "org") == 0)
      {
        // set codeCounter and labelValue to exp
        printf("Setting code counter \n");
        
      }
      else if(strcmp(&lineBuffer[operationStart], "DW") == 0)
      {
        // get length
      }
      else
      {
        // find operation and length
        opcodeIndex = findOpcode(&lineBuffer[operationStart]);
        if(opcodeIndex != 65535)
        {
          opcodeValue = opcodeValues[opcodeIndex];
          opcodeSize = opcodeSizes[opcodeIndex];
        }
        printf("opcode index:%d value: %d size: %d \n",
               opcodeIndex, opcodeValue, opcodeSize);
        codeCounter += opcodeSize;
      }
    }
    if(labelLength != 0)
    {
      symidx = symFind(lineBuffer);
      symInsert(symidx, lineBuffer, 0xa55a);
    }
    
  }while(result != 65535);

  return rtn;
}

/* ***********************************************************************
 * @fn pass2
 * @brief Performs second pass (of two) of the assembly.
 * @return 0 if successful, non-zero on errors.
 * ******************************************************************** */
word pass2(void)
{
  word rtn = 0;
  word result = 0;
  
  codeCounter = 0;
  dataCounter = 0;
  bssCounter = 0;
  pass = 2;

  rewind(infile);

  do
  {
    result = readLine();
  }while(result != 65535);
  

  return rtn;
}

/* ***********************************************************************
 * @fn finalize
 * @brief Finishes off the assembly, performs housekeeping
 * @return zero if successful, non-zero on errors.
 * ******************************************************************** */
word finalize(void)
{
  word rtn = 0;
  fclose(infile);
  fclose(outfile);

  printf("nextString: %d \n", nextString);
  for(int i = 1; i < nextString; i++)
  {
    putchar(strings[i]);
  }
  
  
  return rtn;
}

/* ***********************************************************************
 * @fn getLabel
 * @brief If the line has a label, copy it to string table.
 * @return Length of label,//Strinb table index  of label found, zero if no label.
 * ******************************************************************** */
word getLabel(void)
{
  word rtn = 0;
  word idx = 0;

  if(isalpha(lineBuffer[0]))
  {
    /* labels start in column 0 */
    idx = 0;
    while(isalnum(lineBuffer[idx]))
    {
      idx++;
    }
  }
  lineBuffer[idx] = 0;
  printf("Label is <<<%s>>> \n", lineBuffer);
  rtn = idx;
  nextIndex = idx + 1;

  return rtn;
}

/* ***********************************************************************
 * @fn getOperation
 * @brief If there is an operation on this line, find it.
 * @return Index of operation, zero if none.
 * ******************************************************************** */
word getOperation(void)
{
  word rtn = 0;  // return if no operation found
  word idx = 0;
  word tokenIndex = 0;
  int ch;

  // skip label
  idx = nextIndex;         // Start where we left off with label.
  while(isblank(lineBuffer[idx])) idx++;
  if(isalpha(lineBuffer[idx]))
  {
    rtn = idx;     // Tell the world where operation is.
    operationIndex = idx;  // Tell the WHOLE world!
    while(isalnum(lineBuffer[idx]))
    {
      // TODO add error checking
      idx++;
    }
    lineBuffer[idx] = 0;  /* mark the end of operation */
    nextIndex = idx + 1;
    printf("Operation is: <<<%s>>> \n", &lineBuffer[rtn]); // tokenBuffer);
  }
  
  return rtn;
}

/* ***********************************************************************
 * @fn getOperand
 * @brief If there is an operand, find it.
 * @return 
 * ******************************************************************** */
word getOperand(void)
{
  word rtn = 0;

  return rtn;
}

/* ***********************************************************************
 * @fn symFind
 * @brief Looks for a symbol in the symbol table
 * @param[in] name Pointer to name to look up.
 * @return Index of symbol in table or where it goes.
 * ******************************************************************** */
word symFind(char* name)
{
  word idx = 0;
  word result;

  while(symbols[idx] != 0) // If it 0, this entry is empty
  {
    result = (word) strcmp(name, &strings[symbols[idx]]);
    printf("result: %d\n", result);
    if(result == 0) // match
    {
      break;
    }
    idx += 2;
  }

  printf("idx: %d pointer: %d content: %d\n", idx, symbols[idx], symbols[idx + 1]);
  return idx;
}

/* ***********************************************************************
 * @fn symInsert
 * @brief Inserts a symbol into symbol table if not already there.
 * @param[in] idx Index into string table of name to insert.
 * @param[in] name Pointer to name to insert.
 * @param[in] value The value to associate with the symbol.
 * @return Index of symbol in table or 65535 if already there.
 * ******************************************************************** */
word symInsert(word idx, char* name, word value)
{
  word rtn = 0;
  strcpy(&strings[nextString], name);
  symbols[idx] = nextString;
  nextString += strlen(name) + 1;
  symbols[idx + 1] = value;

  return rtn;
}

/* ***********************************************************************
 * @fn findOpcode
 * @brief Checks a string to see if it is a valid opcode
 * @param[in] str The string to look for
 * @return Table index of opcode or 65535 if not found.
 * ******************************************************************** */
word findOpcode(char* str)
{
  word rtn = 65535;
  word idx;

  idx = 0;
  while(idx < 34)  // 34 opcodes defined right now
  {
    if(strcasecmp(str, opcodeStrings[idx]) == 0)
    {
      // match
      rtn = idx;
      break;
    }
    idx++;
  }

  return rtn;
}

