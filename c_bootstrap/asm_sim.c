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

FILE* infile;                 // Current input file.
FILE* outfile;                // Current output file.


static word lineNumber;       // Line number of input file.
static char lineBuffer[128];  // Current input line.
static word lineIndex;        // Pointer to next char in input line.
word operationIndex;          // Pointer to operation in line buffer.
word operandIndex;            // Pointer to operand(s) in line buffer.

word pass;           // Pass one or two?
word symbols[256];   // Storage for symbols -- symbol table.
char strings[8192];  // Storage for strings -- string table.
word nextSym = 0;    // Where to put next symbol in symbol table.
word nextString = 1; // Where to put next string in string table.
word codeCounter;    // Current offset into code segment.
word dataCounter;    // Current offset into data segement.
word bssCounter;     // Current offset into bss segment.
word* counter;       // Currently in use counter.

word thisTok;        // Current token scanned
word tokValue;       // Value of constant scanned
word err;            // Error code


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
word nextChar(void);
word unNextChar(void);
word scan(void);
word hexValue(word ch);
word expression(void);
word orexp(void);
word xorexp(void);
word andexp(void);
word equalexp(void);
word compexp(void);
word shiftexp(void);
word addexp(void);
word mulexp(void);
word factor(void);


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
  
  printf("%04d: %s\n", lineNumber,lineBuffer);
  lineIndex = 0;   // where to scan next token
  lineNumber++;
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
  word operandLength;
  
  codeCounter = 0;
  dataCounter = 0;
  bssCounter = 0;
  counter = &codeCounter;
  pass = 1;
  lineNumber = 1;

  do
  {
    result = readLine();
    labelLength = getLabel();  /* zero if no label */
    operationStart = getOperation();
    labelValue = *counter; //codeCounter; // tentative
    if(operationStart != 0)
    {
      if(strcasecmp(&lineBuffer[operationStart], "EQU") == 0)
      {
        // set labelValue to exp
        // TODO check if already in table
        thisTok = scan();
        labelValue = expression();
      }
      else if(strcasecmp(&lineBuffer[operationStart], "SET") == 0)
      {
        // set labelValue to exp
        // TODO check if already in table
        thisTok = scan();
        labelValue = expression();
      }
      else if(strcasecmp(&lineBuffer[operationStart], "ORG") == 0)
      {
        // set codeCounter and labelValue to exp
        //printf("Setting code counter to ");
        thisTok  = scan();
        *counter = expression(); // codeCounter = expression();
        printf("%d \n", *counter); // codeCounter);
        labelValue = *counter; // codeCounter;
        
      }
      else if(strcasecmp(&lineBuffer[operationStart], "DW") == 0)
      {
        labelValue = *counter; // codeCounter;
        operandLength = 0;
        // get length
        //thisTok = scan();
        //printf("OP is DW and thisTok is %d \n", thisTok);
        // for each string add length
        // for each expression add 1
        while(1)
        {
          thisTok = scan();
          //printf("in DW loop thisTok is %d \n", thisTok);
          
          if(thisTok == TOK_STRING)
          {
            // get length and add
            // TODO words, not bytes
            operandLength += strlen(&strings[nextString]);
            //printf("String is %s at %d and length %d \n", &strings[nextString],
            //       nextString, operandLength);
            thisTok = scan();
          }
          else
          {
            //printf("DW exp thisTok %d \n", thisTok);
            expression();
            operandLength += 1;
          }
          
          if(thisTok != ',')
          {
            printf("no comma %d \n", thisTok);
            break;
          }
        }
        printf("DW operand length: %d \n", operandLength);
        *counter += operandLength; //codeCounter += operandLength;
      }
      else if(strcasecmp(&lineBuffer[operationStart], "DS") == 0)
      {
        operandLength = expression();
        *counter += operandLength; //codeCounter += operandLength;
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
        //printf("opcode index:%d value: %d size: %d \n",
        //       opcodeIndex, opcodeValue, opcodeSize);
        *counter += opcodeSize; // codeCounter += opcodeSize;
      }
    }
    if(labelLength != 0)
    {
      symidx = symFind(lineBuffer);
      symInsert(symidx, lineBuffer, labelValue);
    }
    //printf("Line index %d \n", lineIndex);
    
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
  counter = &codeCounter;
  pass = 2;
  rewind(infile);
  lineNumber = 1;

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

  //printf("nextString: %d \n", nextString);
  for(int i = 1; i < nextString; i++)
  {
    //putchar(strings[i]);
  }
  printf("\n\nSymbol Table \n");
  word sym = 0;
  while(symbols[sym] != 0)
  {
    printf("%s: %04x \n", &strings[symbols[sym]], symbols[sym + 1]);
    sym += 2;
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
  lineIndex = idx + 1;

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
  idx = lineIndex;         // Start where we left off with label.
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
    lineIndex = idx + 1;
    //printf("Operation is: <<<%s>>> \n", &lineBuffer[rtn]);
    //printf("lineIndex is: %d \n", lineIndex);
  }
  
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
    //printf("result: %d\n", result);
    if(result == 0) // match
    {
      break;
    }
    idx += 2;
  }

  //printf("idx: %d pointer: %d content: %d\n",idx,symbols[idx],symbols[idx+1]);
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

/* ***********************************************************************
 * @fn nextChar
 * @brief Get next character from input line.
 * @return Next character or 65535/EOF if none.
 * ******************************************************************** */
word nextChar(void)
{
  word rtn = 0;
  rtn = lineBuffer[lineIndex];
  if(rtn == 0) rtn = 65535;
  else lineIndex++;
  

  return rtn;
}

/* ***********************************************************************
 * @fn unNextChar
 * @brief Return last char to input stream.
 * @return 
 * ******************************************************************** */
word unNextChar(void)
{
  word rtn = 0;
  lineIndex--;    // Assume we know what we're doing

  return rtn;
}

/* ***********************************************************************
 * @fn isIdStart
 * @brief Check if if this character can start an identifier/label
 * @param[in] ch Character to check.
 * @return true if this character can start an identifier.
 * ******************************************************************** */
word isIdStart(word ch)
{
  word rtn = 0;
  if(ch == '_')
  {
    rtn = 1;
  }
  else if(ch >= 'a' && ch <= 'z')
  {
    rtn = 1;
  }
  else if(ch >= 'A' && ch <= 'Z')
  {
    rtn = 1;
  }

  return rtn;
}

/* ***********************************************************************
 * @fn hexValue
 * @brief Get value of a hex digit.
 * @param[in] ch Character to take value of.
 * @return Value of hex character.
 * ******************************************************************** */
word hexValue(word ch)
{
  word rtn = 0;
  
  if(ch >= '0' && ch <= '9')
  {
    rtn = ch - '0';
  }
  else if(ch >= 'A' && ch <= 'F')
  {
    rtn = ch - 'A' + 10;
  }
  else if(ch >= 'a' && ch <= 'f')
  {
    rtn = ch - 'a' + 10;
  }

  return rtn;
}


/* ***********************************************************************
 * Tokens
 *  IDENT = (letter | _ ) { (letter | _ | digit) }
 *  CONST = digit { digit } | $ hexdigit { hexdigit} | 'char char'
 *  STRING = "{char}".
 *  << >> < <= > >= == != + - * / % & | ^ ! ~ 
 * ******************************************************************** */
/* ***********************************************************************
 * @fn scan
 * @brief Get next token from input line.
 * @return Token scanned or 65535 (TOK_NONE)
 * ******************************************************************** */
word scan(void)
{
  word rtn = 0;
  word ch;
  word ch2;
  word strptr;

  tokValue = 0;
  
  ch = nextChar();
  // skip white
  while(ch == ' ' || ch == '\t')
  {
    ch = nextChar();
  }
  
  // assume a single character token at first
  rtn = ch;
  // then check for others
  if(isIdStart(ch))                             /* Identifier */
  {
    rtn = TOK_IDENT;
    strptr = nextString;
    while(isIdStart(ch) || isdigit(ch))
    {
      strings[nextString] = ch;
      nextString++;
      ch = nextChar();
    }
    unNextChar();  // put back non ident char
  }
  else if(ch == '$')                            /* Hex integer */
  {
    rtn = TOK_NUM;
    ch = nextChar();
    while(isxdigit(ch))
    {
      tokValue = tokValue * 16 + hexValue(ch);
      ch = nextChar();
    }
    unNextChar(); // Put back the non hex char.
  }
  else if(isdigit(ch))                          /* Decimal integer */
  {
    rtn = TOK_NUM;
    while(isdigit(ch))
    {
      tokValue = tokValue * 10 + (ch - '0');
      ch = nextChar();
    }
    unNextChar(); // put back non digit.
  }
  else if(ch == '\'')                           /* char constant */
  {
    rtn = TOK_NUM;
    ch = nextChar();
    // TODO error checking
    // TODO hmmmm, how to handle this
    while(ch != '\'' && ch != 65535)
    {
      tokValue <<= 8;
      tokValue |= (ch & 0xff);
      ch = nextChar();
    }
    if(ch != '\'')
    {
      //error
    }
    printf("Char const: %04X\n", tokValue);

  }
    
  else if(ch == '"')                            /* string */
  {
    // Save the string in the string table but don't update pointer.
    rtn = TOK_STRING;
    strptr = nextString;
    tokValue = nextString;
    ch = nextChar();
    while(ch != '"' && ch != 65535)
    {
      // TODO escapes
      strings[strptr] = ch;
      strptr++;
      ch = nextChar();
    }
    if(ch != '"')
    {
      // error
    }
    strings[strptr] = 0;  /* terminate string */
  }
  else if(ch == '<')                            /* <, <=, << */
  {
    ch2 = nextChar();
    if(ch2 == '=')
    {
      rtn = TOK_LESS_EQ;
    }
    else if(ch2 == '<')
    {
      rtn = TOK_SHL;
    }
    else
    {
      unNextChar();  /* put it back */
    }
  }
  else if(ch == '>')                            /* >, >=, >> */
  {
    ch2 = nextChar();
    if(ch2 == '=')
    {
      rtn = TOK_GREAT_EQ;
    }
    else if(ch2 == '>')
    {
      rtn = TOK_SHR;
    }
    else
    {
      unNextChar(); /* put it back */
    }
  }
  else if(ch == '=')                            /* =, == */
  {
    ch2 = nextChar();
    if(ch2 == '=')
    {
      rtn = TOK_EQ_EQ;
    }
    else
    {
      unNextChar(); /* put it back */
    }
  }
  else if(ch == '!') /* !, != */
  {
    ch2 = nextChar();
    if(ch2 = '=')
    {
      rtn = TOK_NOT_EQ;
    }
    else
    {
      unNextChar(); /* put it back */
    }
  }
  return rtn;
}

/* ***********************************************************************
 * expressions = orexp.
 * orexp = xorexp [ | xorexp ].
 * xorexp = andexp [ ^ andexp ].
 * andexp = equalexp { & equalexp }.
 * equalexp = compexp { equalop compexp }.    equalop == !=
 * compexp = shiftexp { compop shiftexp }.    compop < <= > >=
 * shiftexp = addexp { shiftop addexp }.
 * addexp = mulexp { addop mulexp }.
 * mulexp = factor { mulop factor }.
 * factor = 
 * ******************************************************************** */

/* ***********************************************************************
 * @fn expression
 * @brief Parse an expression, return the value
 * @return The value of the expression
 * ******************************************************************** */
word expression(void)
{
  word rtn;
  
  rtn = orexp();
  //printf("exp: %d\n", rtn);
  return rtn;
}

/* ***********************************************************************
 * @fn orexp
 * @brief orexp = xorexp [ | xorexp ].
 * @return Expression value.
 * ******************************************************************** */
word orexp(void)
{
  word rtn = 0;
  word op;
  word temp;
  
  rtn = xorexp();
  while(thisTok == '|')
  {
    thisTok = scan();
    temp = xorexp();
    rtn = rtn | temp;
  }

  //printf("orexp: %d\n", rtn);
  return rtn;
}

/* ***********************************************************************
 * @fn xorexp
 * @brief  xorexp = andexp [ ^ andexp ].
 * @return Expression value.
 * ******************************************************************** */
word xorexp(void)
{
  word rtn = 0;
  word op;
  word temp;
  
  rtn = andexp();
  while(thisTok == '^')
  {
    op = thisTok;
    thisTok = scan();
    temp = andexp();
    rtn = rtn ^ temp;
  }

  //printf("xorexp: %d \n", rtn);
  return rtn;
}

/* ***********************************************************************
 * @fn andexp
 * @brief andexp = equalexp { & equalexp }. bitwise AND
 * @return Expression value
 * ******************************************************************** */
word andexp(void)
{
  word rtn = 0;
  word op;
  word temp;
  
  rtn = equalexp();
  while(thisTok == '&')
  {
    op = thisTok;
    thisTok = scan();
    temp = equalexp();
    rtn = rtn & temp;
  }

  //printf("andexp: %d \n", rtn);
  return rtn;
}

/* ***********************************************************************
 * @fn equalexp
 * @brief  equalexp = compexp { equalop compexp }.    equalop == !=
 * @return Expression value.
 * ******************************************************************** */
word equalexp(void)
{
  word rtn = 0;
  word op;
  word temp;
  
  rtn = compexp();
  while(thisTok == TOK_EQ_EQ || thisTok == TOK_NOT_EQ)
  {
    op = thisTok;
    thisTok = scan();
    temp = compexp();
    if(op == TOK_EQ_EQ)
    {
      if(rtn == temp) rtn = 1;
      else rtn = 0;
    }
    else if(op == TOK_NOT_EQ)
    {
      if(rtn != temp) rtn = 0;
      else rtn = 1;
    }
  }

  //printf("equalexp: %d \n", rtn);
  return rtn;
}

/* ***********************************************************************
 * @fn compexp
 * @brief compexp = shiftexp { compop shiftexp }.    compop < <= > >=
 * @return Expression value.
 * ******************************************************************** */
word compexp(void)
{
  word rtn = 0;
  word op;
  word temp;

  rtn = shiftexp();
  while(thisTok == '<' || thisTok == '>' || thisTok == TOK_LESS_EQ
        || thisTok == TOK_GREAT_EQ)
  {
    op = thisTok;
    thisTok = scan();
    temp = shiftexp();
    if(op == '<')
    {
      if(rtn < temp) rtn = 1;
      else rtn = 0;
    }
    else if(op == TOK_LESS_EQ)
    {
      if(rtn <= temp) rtn = 1;
      else rtn = 0;
    }
    else if(op == '>')
    {
      if(rtn > temp) rtn = 1;
      else rtn = 0;
    }
    else if(op == TOK_GREAT_EQ)
    {
      if(rtn >= temp) rtn = 1;
      else rtn = 0;
    }
  }

  //printf("compexp: %d \n", rtn);
  return rtn;
}

/* ***********************************************************************
 * @fn shiftexp
 * @brief shiftexp = addexp { shiftop addexp }.
 * @return Expression value.
 * ******************************************************************** */
word shiftexp(void)
{
  word rtn = 0;
  word op;
  word temp;

  rtn = addexp();
  while(thisTok == TOK_SHL || thisTok == TOK_SHR)
  {
    op = thisTok;
    thisTok = scan();
    temp = addexp();
    if(op == TOK_SHL) rtn = rtn << temp;
    else if(op == TOK_SHR) rtn = rtn >> temp;
  }

  //printf("shiftexp: %d\n", rtn);
  return rtn;
}


/* ***********************************************************************
 * @fn addexp
 * @brief addexp = mulexp { addop mulexp }.
 * @return Expression value.
 * ******************************************************************** */
word addexp(void)
{
  word rtn = 0;
  word op;
  word temp;

  rtn = mulexp();
  while(thisTok == '+' || thisTok == '-')
  {
    op = thisTok;
    thisTok = scan();
    temp = mulexp();
    if(op == '+') rtn = rtn + temp;
    else if(op == '-') rtn = rtn - temp;
  }

  //printf("addexp: %d \n", rtn);
  return rtn;
}


/* ***********************************************************************
 * @fn mulexp
 * @brief mulexp = factor { mulop factor }.
 * @return Expression value.
 * ******************************************************************** */
word mulexp(void)
{
  word rtn = 0;
  word op;
  word temp;

  rtn = factor();
  while(thisTok == '*' || thisTok == '/' || thisTok == '%')
  {
    op  = thisTok;
    thisTok = scan();
    temp = factor();
    if(op == '*') rtn = rtn * temp;
    else if(op == '/') rtn = rtn / temp;
    else if(op == '%') rtn = rtn % temp;
  }

  //printf("mulexp: %d \n", rtn);
  return rtn;
}

/* ***********************************************************************
 * @fn factor
 * @brief factor = name | constant | ( exp ) | unary factor
 * @return Expression value.
 * ******************************************************************** */
word factor(void)
{
  word rtn = 0;
  printf("factor first tok %d \n", thisTok);

  if(thisTok == TOK_NUM)
  {
    rtn = tokValue;
    thisTok = scan();
  }
  else if(thisTok == TOK_IDENT)
  {
    // TODO look up value
    rtn = 0xDEAD;
    thisTok = scan();
  }
  else if(thisTok == '(')
  {
    thisTok = scan();
    rtn = expression();
    if(thisTok != ')') // error
    {
      err = 1;
    }
    thisTok = scan();
  }
  else if(thisTok == '-')
  {
    thisTok = scan();
    rtn = 0 - factor();
  }
  else if(thisTok == '~')
  {
    thisTok = scan();
    rtn = ~factor();
  }
  else if(thisTok == '!')
  {
    thisTok = scan();
    rtn = factor();
    if(rtn == 0)
    {
      rtn = 1;
    }
    else
    {
      rtn = 0;
    }
  }

  printf("factor: %d \n", rtn);
  return rtn;
}

