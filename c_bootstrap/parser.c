
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

#include "definitions.h"
#include "parser.h"
#include "scanner.h"
#include "symtable.h"
#include "codegen.h"


// typedef int16_t word;

word tok;
char localText[32];

#define UNVALUE   0
#define RVALUE    1
#define LVALUE    2

word valueType = UNVALUE;       /* What type of value is current? */
word inFunction = 0;            /* Are we parsing a function? */

word errorCount = 0;          /* How many errors have we found */
word warningCount = 0;        /* How many warning have we found */

word dataCounter = 0;       /* Where to compile next data item */
word bssCounter = 0;        /* Where to compile next bss item */
word codeCounter = 0;       /* Where to compile next code item */

word skipTo(word nextToken);

/* ***********************************************************************
 * @fn parse
 * @brief parses and compiles an input file 
 * @param f Does nothing for now
 * @return Compile status: 0 for clean compile 
 * ******************************************************************** */
word parse( int f )
{
  word rtn = 0;

  // initialize
  inFunction = 0;
  errorCount = 0;
  warningCount = 0;
  dataCounter = 0;
  bssCounter = 0;
  codeCounter = 0;
  
  word result = program();
  
  // finalize
  sym_dumpTable();
  printf("There were %d errors found. \n", errorCount);
  rtn = errorCount;

  return rtn;
}

/* ***********************************************************************
 * @fn program
 * @brief parses a program: { definition } .
 * @return parse status
 * ******************************************************************** */
word program()
{
  word rtn = 0;
  inFunction = 0;
  
  tok = scan();
  printf("Program\n");
  do
  {
    definition();
  }while(tok != TOK_EOF);
  
  return rtn;
}

/* ***********************************************************************
 * @fn definition
 * @brief parses a definition: variable, vector, function
 * @return parse status
 * ******************************************************************** */
word definition()
{
  word rtn = 0;
  word count = 0;    /* how many params or initializers */
  word flags;
  word value;
  word stackCounter;  /* auto/parameter offset from paramptr/frameptr */
  
  if(tok == TOK_IDENT) /* name */
  {
    /* TODO: ... */
    /* If it's an ident we need the name */
    strcpy(localText, getText());
    tok = scan();
    
    /* ********** Vectors ********** */
    if(tok == '[')    /* vector */
    {
      /* vector */
      /* constant ] ival {, ival } */
      sym_insert(localText, FLAG_EXTRN | FLAG_VEC, dataCounter++);
      tok = scan();
      if(tok == TOK_INT)  /* constant */
      {
        /* process */
        tok = scan();
        if(tok != ']')    /* error: expected ] */
        {
          /* error routine */
          errorCount++;
        }
        tok = scan();     /* look for initializers */
        while(tok == TOK_INT || tok == TOK_IDENT || tok == TOK_STRING)
        {
          /* ... */
          /* process constant or name */
          tok = scan();
          if(tok != ',')
          {
            break;
          }
        }
        if(tok != ';')
        {
          rtn = -1;
          /* ERROR */
          /* TODO */
          errorCount++;
        }
        tok = scan();
      /* ... */
      }
      
    }

    /* ********** Functions ********** */
    else if(tok == '(') /* function */
    {
      /* [ name { , name } ] ) statement */
      
      sym_insert(localText, FLAG_FUNCTION, codeCounter);
      if(inFunction)       /* A function inside another is not allowed */
      {
        rtn = -1;
        /* ERROR */
        errorCount++;
      }
      inFunction = 1;
      tok = scan();
      stackCounter = 0;
      flags = FLAG_PARAMETER;
      
      while(tok == TOK_IDENT)
      {
        /* process name */
        strcpy(localText, getText());
        sym_insert(localText, flags, stackCounter);
        stackCounter++;
        
        tok = scan();
        if(tok != ',')
        {
          break;
        }
        else
        {
          tok = scan();
        }
      }
      if(tok == ')')
      {
        tok = scan();
        statement();
      }
      else
      {
        /* error */
        errorCount++;
        rtn = -1;
      }
      inFunction = 0;     /* Tell the world we are done */
    }

    /* ********** Initialized Variable ********** */
    else if(tok == TOK_INT || tok == TOK_IDENT || tok == TOK_STRING)
    {
      tok = scan();
      if(tok != ';')
      {
        printf("Error: expected ; \n");
        errorCount++;
        rtn = -1;
      }
      else
      {
        tok = scan();
      }
    }

    /* ********** Simple, uninitialized variable ********** */
    else if(tok == ';')
    {
      /* simple variable */
      value = dataCounter;
      dataCounter++;
      flags = FLAG_EXTRN;
      sym_insert(localText, flags, value);
      tok = scan();
    }
  }
  else /* error: expected a name to define */
  {
    /* error routine */
    printf("Definition Error - expected name to define \n");
    printf("Token is: %d\n", tok);
    printf("Token '%s' on line: %d\n", getText(), getLineNumber());
    while(1);
    rtn = -1;
    errorCount++;
    exit(1);
  }

  return rtn;
}

/* ***********************************************************************
 * @fn statement
 * @brief parses a statement
 * @return parse status
 * ******************************************************************** */
word statement()
{
  word rtn = 0;
  word flags;
  word value;
  word br1;
  word br2;

  /* TODO: fill this in */
  while(tok == TOK_AUTO || tok == TOK_EXTRN
        || tok == TOK_CASE ) // TODO labels || tok == TOK_IDENT)
  {
    switch(tok)
    {
    case TOK_AUTO:                         /* keyword auto */
      tok = scan();
      if(tok != TOK_IDENT)
      {
        /* error */
        rtn = -1;
        errorCount++;
        tok = scan(); /* what now? */
      }
      else                                 /* got name */
      {
        /* put into table, on stack */
        strcpy(localText, getText());
        tok = scan();
        if(tok == TOK_INT || tok == TOK_STRING || tok == TOK_IDENT) /* initializer? */
        {
          sym_insert(localText, FLAG_AUTO, dataCounter++);
          /* TODO initialize and write data block to .obj */
          tok = scan();
          while(tok == ',')
          {
            tok = scan();    /* get value */
            /* initialize */
            tok = scan();    /* get next */
          }
          if(tok != ';')
          {
            /* ERROR */
            printf("Initializer expected ';' \n");
            while(1);
            rtn = -1;
            errorCount++;
          }
          else
          {
            sym_insert(localText, FLAG_AUTO, dataCounter++); /* TODO extrn? */
            tok = scan();
          }
        }
        else if(tok == ';')  /* not initialized, look for terminator */
        {
          
          sym_insert(localText, FLAG_AUTO, dataCounter++);

          tok = scan();
        }
      }
      /* todo */
      break;
    case TOK_EXTRN:
      tok = scan();
      if(tok != TOK_IDENT)
      {
        /* error */
        /* TODO what else here? scan? */
        rtn = -1;
        errorCount++;
      }
      else
      {
        strcpy(localText, getText());
        sym_insert(localText, FLAG_EXTRN, 0); /* TODO: output undef ref here?? */
        
        tok = scan();
        if(tok != ';')
        {
          /* error */
          /* TODO what else here? */
          rtn = -1;
          errorCount++;
        }
      }
      break;
    case TOK_CASE:
      tok = scan();
      /* todo */
      break;
      
    case TOK_IDENT:         /* TODO: add later see while above  labels 'name:' */
      printf("BOGUS!!!!!!!\n");
      tok = scan();
      /* todo */
      break;
    }
  }
  switch(tok)
  {
  case TOK_IF:  /* if( rvalue ) statement [ else statement ] */
    tok = scan();
    if(tok != '(')
    {
      /* error */
      rtn = -1;
      break;
    }
    tok = scan();
    expression();
    if(tok != ')')
    {
      /* error */
      printf("IF ST ERROR no ) found %d \n", tok);
      rtn = -1;
      break;
    }
    br1 = gen_if_start();
    tok = scan();
    statement();
    // TODO ! fix gen_if_else();
    if(tok == TOK_ELSE)
    {
      br1 = gen_if_else(br1);
      tok = scan();
      statement();
    }
    gen_if_end(br1);
    /* todo */
    break;
    
  case TOK_WHILE:  /* while '(' rvalue ')' statement */
    br1 = gen_while_start();
    tok = scan();
    if(tok != '(')
    {
      /* error */
      rtn = -1;
      break;
    }
    tok = scan();
    expression();
    if(tok != ')')
    {
      /* error */
      rtn = -1;
      break;
    }
    br2 = gen_while_exp();
    // TODO fix this gen_while_end();
    tok = scan();
    statement();
    /* todo */
    gen_while_end(br1, br2);
    break;
    
  case TOK_SWITCH:  /* switch rvalue statement */
    tok = scan();
    if(tok != '(')
    {
      /* error */
      rtn = -1;
      break;
    }
    tok = scan();
    expression();
    if(tok != ')')
    {
      /* error */
      rtn = -1;
      break;
    }
    tok = scan();
    statement();
    /* todo */
    break;
    
  case TOK_GOTO:   /* goto rvalue ; */
    tok = scan();
    /* todo */
    break;
  case TOK_RETURN: /* return [ (rvalue) ] ; */
    tok = scan();
    if(tok != ';')
    {
      /* ERROR */
      expression();
    }
    // TODO should be inside above if or something 
    if(tok != ';')
    {
      /* ERROR  expected ; */
      rtn = -1;
    }
    tok = scan();
    break;
  case '{':        /* '{' {statement} '}' */
    tok = scan();
    while(tok != '}')
    {
      statement();
    }
    tok = scan();
    break;
   
  default:         /* [rvalue] ; */
    expression();
    if(tok == ';')                  /* Good, found the end */
    {
      tok = scan();
      break;
    }
    else                            /* Nope! Error */
    {
      printf("Error:  Expected ';' after expression statement \n");
      while(1);
      break;
    }
    /* todo rvalue / null statement */
    /* and error */
    /* if tok != ';' then rvalue then ';' */
    break;
  }
  

  return rtn;
}

/* ***********************************************************************
 * @fn expression
 * @brief parses a expression
 * @return parse status
 * ******************************************************************** */
word expression()
{
  word rtn = 0;
  assignexp();

  /* TODO: */

  return rtn;
}

/* ***********************************************************************
 * @fn isassignop
 * @brief Checks is a token is an assignment operator
 * @return If it is assignment op, return 1, else 0
 * ******************************************************************** */
word isassignop(word t)
{
  word rtn = 0;

  switch(t)
  {
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
    rtn = 1;
  }
  return rtn;
}

/* ***********************************************************************
 * @fn assignexp
 * @brief parses an assign expression: conditionalexp { assop conditional}
 * @return parse status
 * ******************************************************************** */
word assignexp()
{
  word rtn = 0;
  word thisTok;

  /* TODO */   /* cond() while( is assign) { assignexp() } */
  conditionalexp();
  while( isassignop(tok))
  {
    /* TODO */
    thisTok = tok;
    tok = scan();
    assignexp();
    //printf("\tDOASSIGN_%03d\n",thisTok);
    gen_binop(thisTok);
  }

  return rtn;
}

/* ***********************************************************************
 * @fn conditionalexp
 * @brief parses conditional exp: orexp { orop orexp }
 * @return parse status
 * ******************************************************************** */
word conditionalexp()
{
  word rtn = 0;
  word trueValue = RVALUE;
  word falseValue = RVALUE;
  /* TODO */
  /* RVALUE/LVALUE */
  /* rest of function */
  word addr1;
  word addr2;

  rtn = orexp();
  if(tok == '?')
  {
    // printf("\tJMZ\tFALSEPART\t\t;If cond exp was false, jump over true part\n");
    addr1 = gen_cond_true();
    tok = scan();
    rtn = orexp();
    trueValue = valueType;  /* Get true part value */
    // printf("FALSEPART: \n");
    addr2 = gen_cond_false(addr1);
    if(tok == ':')
    {
      tok = scan();
      rtn = orexp();
      falseValue = valueType; /* Get false part value */
      // printf("\tJMP\tENDOFCOND\n");
      gen_cond_end(addr2);
    }
    else
    {
      /* error */
      rtn = -1;
      printf("\nPARSE ERROR: expected ':' in conditional expression\n");
    }

    /* ???? more? */
    if(trueValue == LVALUE && falseValue == LVALUE)
    {
      valueType = LVALUE; /* defaulted to RVALUE */
    }
    //printf("ENDOFCOND\n");
  }
    
  return rtn;
}

/* ***********************************************************************
 * @fn orexp
 * @brief parses an OR expression: andexp { | andexp }
 * @return parse status 
 * ******************************************************************** */
word orexp()
{
  word rtn = 0;

  rtn = andexp();
  while(tok == '|')
  {
    valueType = RVALUE;
    tok = scan();
    rtn = andexp();
    gen_binop('|');
  }

  return rtn;
}

/* ***********************************************************************
 * @fn andexp
 * @brief parses an AND expression: equalexp { & equalexp }
 * @return parse status
 * ******************************************************************** */
word andexp(void)
{
  word rtn = 0;

  rtn = equalexp();
  while(tok == '&')
  {
    valueType = RVALUE;
    tok = scan();
    rtn = equalexp();
    gen_binop('&');
  }

  return rtn;
}

/* ***********************************************************************
 * @fn equalexp
 * @brief parses an EQUALITY expression: relateexp { == | != relateexp }
 * @return parse status
 * ******************************************************************** */

word equalexp(void)
{
  word rtn = 0;
  word thisTok;

  rtn = relateexp();
  while(tok == TOK_EQ_EQ | tok == TOK_NOT_EQ)
  {
    thisTok = tok;
    valueType = RVALUE;
    tok = scan();
    rtn = relateexp();
    gen_binop(thisTok);
  }

  return rtn;
}

/* ***********************************************************************
 * @fn relateexp
 * @brief parses a relate expression: shiftexp { relateop shiftexp }
 * @return parse status
 * ******************************************************************** */

word relateexp()
{
  word rtn = 0;
  word thisTok;

  rtn = shiftexp();
  while( tok == '<' | tok == '>'
         | tok == TOK_LESS_EQ | tok == TOK_GREAT_EQ )
  {
    thisTok = tok;
    valueType = RVALUE;
    tok = scan();
    rtn = shiftexp();
    gen_binop(thisTok);
  }
  

  return rtn;
}

/* ***********************************************************************
 * @fn shiftexp
 * @brief parses a shift expression: addexp { addop addexp }
 * @return parse status
 * ******************************************************************** */

word shiftexp()
{
  word rtn = 0;
  word thisTok;

  rtn = addexp();
  while(tok == TOK_SHIFT_LEFT | tok == TOK_SHIFT_RIGHT)
  {
    thisTok = tok;
    valueType = RVALUE;
    tok = scan();
    rtn = addexp();
    gen_binop(thisTok);
  }
  

  return rtn;
}

/* ***********************************************************************
 * @fn addexp
 * @brief parses an add expression: term { addop term }
 * @return parse status
 * ******************************************************************** */
word addexp()
{
  word rtn = 0;
  word thisTok;

  rtn = term();
  while(tok == '+' | tok == '-')
  {
    thisTok = tok;
    valueType = RVALUE;
    tok = scan();
    rtn = term();
    gen_binop(thisTok);
  }

  return rtn;
}

/* ***********************************************************************
 * @fn term
 * @brief parses a term: factor { mulop factor }
 * @return parse status
 * ******************************************************************** */
word term()
{
  word rtn;
  rtn = 0;
  word thisTok;

  rtn = factor();
  while(tok == '*' | tok == '/' | tok == '%')
  {
    thisTok = tok;
    valueType = RVALUE;
    tok = scan();
    rtn = factor();
    gen_binop(thisTok);
    
  }

  return rtn;
}

/* ***********************************************************************
 * @fn factor
 * @brief parses a factor 
 * @return parse status
 * ******************************************************************** */
/* ***********************************************************************
 *  factor = rvalue
 *  rvalue = ( rvalue )                 
 *         | lvalue
 *         | constant                decimal, octal, char, string
 *         | lvalue assign rvalue               *** expression
 *         | inc-dec lvalue                     *** expression
 *         | lvalue inc-dec                     *** expression
 *         | unary rvalue                       *** expression
 *         | & lvalue                           *** expression
 *         | rvalue binary rvalue               *** expression
 *         | rvalue ? rvalue : rvalue           *** expression
 *         | rvalue ( [ rvalue { , rvalue } ] ) *** expression
 *
 *  lvalue = name
 *         | * rvalue
 *         | rvalue[ rvalue ]
 *************************************************************************/

word factor() /* rvalue */
{
  word rtn;
  word symIndex;
  word flags;
  word value;
  word thisTok;
  
  rtn = 0;
  word val;
  // TODO word tokChars;
  char* tokChars;
  
  switch(tok)
  {
    /*   Constant: literal integer / character */
  case TOK_INT:           /* dec, oct, char, : rvalue */
    valueType = RVALUE;
    val = getNumber();
    tok = scan();

    /* If followed by '[' then it's a vector reference */
    if(tok == '[')        /* vector */
    {
      valueType = LVALUE;
      tok = scan();
      expression();
      /* TODO */
      if(tok != ']')
      {
        rtn = -1;
        /* error */
      }
      tok = scan();
    }
    
    /* If followed by '(' then it's a function call */
    else if(tok == '(')   /* function call */
    {
      valueType = RVALUE;
      tok = scan();
      /* paramlist */
      /* ... */
      /* TODO */
      gen_function_call(val); // printf("\tCALL\t%04d\n",val);
    }
    else   /* just a number */
    {
      gen_literal(val); //printf("\tPSHI\t%04d\n", val);
    }
    
    break;
    
  case TOK_STRING:        /* "cccc..."   :rvalue */
    valueType = RVALUE;
    val = getNumber();
    tok = scan();
    gen_literal(val); // TODO printf("\tPUSH\t%04d\t\t;Push string address\n",val);
    break;
    
  case TOK_IDENT:         /* name */
    valueType = LVALUE;
    tokChars = getText();
    /* TODO convert this to B */
    strcpy(localText,getText()); /* TODO either call getText or just use tokChars */
    tok = scan();
    if(tok == '[')         /* vector */
    {
      valueType = LVALUE;  /* STILL! */
      tok = scan();
      /* expression */
      /* TODO */
      if(tok != ']')
      {
        rtn = -1;
        /* error */
      }
      tok = scan();
    }
    else if(tok == '(')      /* function call */
    {
      //printf("Compiling fn() \n");
      valueType = RVALUE;
      //word address = lookup(name);
      word address = 1234;
      tok = scan();
      /* paramlist */
      /* ... */
      /* TODO */
      if(tok != ')')
      {
        /* ERROR */
        printf("Error token is %d \n", tok);
        rtn = -1;
      }
      //printf("\tCALL\t%5d\n", address);
      gen_function_call(0);
      tok = scan();
    }
    else /* just a plain variable */
    {
      // TODO THESE PUSH the ADDRESS of the VARIABLE !!! 
      symIndex = sym_find(localText);
      flags = sym_getFlags(symIndex);
      value = sym_getValue(symIndex);
      
      if(flags & FLAG_EXTRN | flags & FLAG_FUNCTION)  /* extrn so absolute address */
      {
        gen_literal(value); // TODO , localText);
        /* TODO relocation of extrn address */
      }
      else if(flags & FLAG_AUTO)    /* A local var */
      {
        gen_get_parameter(value);
      }
      else if(flags & FLAG_PARAMETER)  /* A parameter */
      {
        gen_get_auto(value);
      }
      else
      {
        /* Something is wrong */
        /* ERROR */
        rtn = -1;
      }
    }
    break;

  case '-':                /* negation  */
  case '!':                /* logical NOT */
  case '~':                /* bitwise NOT */
  case '&':                /* address of */
  case '*':                /* indirection */
    thisTok = tok;
    tok = scan();
    factor();
    gen_unary(thisTok);
    break;

  default:
    rtn = -1;
    /* error */
  }
  
    
  return(rtn);
}

/* ***********************************************************************
 * @fn skipTo
 * @brief Error recover: skips tokens until it scans nextToken or EOF.
 * @param nextToken The token to skip to/over.
 * @return Dunno yet
 * ******************************************************************** */
word skipTo(word nextToken)
{
  word rtn = 0;
  while((tok = scan()) != nextToken);


  return rtn;
}

/* ***********************************************************************
 * @fn main
 * @brief Oversee compilation of a single file.
 * @param[in] argc Number of arguments passed from command line
 * @param[in] argv Array of pointers to the arguments
 * @return Error code: 0 for success, or error code.
 * ******************************************************************** */
int main(int argc, char** argv)
{
  int rtn = 0;
  int infile = -1;
  if(argc > 1)
  {
    if(! scanfile(argv[1]))
    {
      printf("Parsing file %s ... \n", argv[1]);
      sym_initialize();
      parse( 0 );
      printf("Done.\n");
    }
    else
    {
      printf("Couldn't open %s \n", argv[1]);
      rtn = 1;
    }
    
  }
  else
  {
    printf("No input file specified, exiting. \n");
      exit(1);
  }
  return 0;
}



    

