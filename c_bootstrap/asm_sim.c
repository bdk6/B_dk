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


static char lineBuffer[256];
static char baseName[128];
static char outFileName[128];
FILE* infile;
FILE* outfile;

word symbols[256];
char strings[8192];
word nextSym = 0;
word nextString = 0;

word codeCounter;
word dataCounter;
word bssCounter;

word initialize(char* filename);
word readLine(void);
word pass1(void);
word pass2(void);
word finalize(void);
word getLabel(void);
word getOperation(void);
word getOperand(void);

int main(int argc, char** argv)
{
  int rtn = 0;
  word result;
  printf("Starting assembly.\n");
  if(initialize(argv[1]))
    {
      printf("Exiting...\n");
    }
  //while(readLine() != 65535);
  printf("Starting pass 1 \n");
  result = pass1();
  printf("Result is %d \n", result);
  if(!result)
  {
    printf("Starting pass 2\n");
    result = pass2();
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
    strcpy(outFileName, filename);
    len = strlen(filename);
    for(idx = len; idx > 0; idx--)
    {
      if(outFileName[idx] == '.')
      {
        outFileName[idx] = 0;
        break;
      }
    }
    strcpy(outFileName, ".bin");
    infile = fopen(filename, "r");
    outfile = fopen(outFileName, "w");

    for(idx = 0; idx < 256; idx++)
    {
      symbols[idx] = 0;
    }
    for(idx = 0; idx < 8192; idx++)
    {
      strings[idx] = 0;
    }
    nextSym = 0;
    nextString = 0;

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

word readLine(void)
{
  word rtn;
  word idx;
  int ch;
  idx = 0;
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
  return rtn;
}


word pass1(void)
{
  word rtn = 0;
  word result = 0;
  
  codeCounter = 0;
  dataCounter = 0;
  bssCounter = 0;

  do
  {
    result = readLine();
    getLabel();
    
  }while(result != 65535);

  return rtn;
}

word pass2(void)
{
  word rtn = 0;
  word result = 0;
  
  codeCounter = 0;
  dataCounter = 0;
  bssCounter = 0;

  rewind(infile);

  do
  {
    result = readLine();
  }while(result != 65535);
  

  return rtn;
}

word finalize(void)
{
  word rtn = 0;
  fclose(infile);
  fclose(outfile);
  

  return rtn;
}


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
      strings[nextString + idx] = lineBuffer[idx];
      idx++;
    }
    strings[nextString + idx] = 0;  /* NULL term */
    printf("Label is <<<%s>>> \n", &strings[nextString]);
  }
  rtn = idx;

  return rtn;
}

word getOperation(void)
{
  word rtn = 0;

  return rtn;
}

word getOperand(void)
{
  word rtn = 0;

  return rtn;
}


