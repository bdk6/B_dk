
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

// typedef int16_t word;

word tok;

#define UNVALUE   0
#define RVALUE    1
#define LVALUE    2

word valueType = UNVALUE;       /* What type of value is current? */
word inFunction = 0;            /* Are we parsing a function? */

word parse( int f )
{
  word rtn = 0;

  // initialize
  word result = program();
  // finalize

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
//  printf("Starting Definition \n");
  if(tok == TOK_IDENT) /* name */
  {
    /* TODO: ... */
//    printf("Found Name ");
    tok = scan();
    
    /* ********** Vectors ********** */
    if(tok == '[')    /* vector */
    {
      /* vector */
      /* constant ] ival {, ival } */
//      printf("Vector ");
      tok = scan();
      if(tok == TOK_INT)  /* constant */
      {
        /* process */
        tok = scan();
        if(tok != ']')    /* error: expected ] */
        {
          /* error routine */
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
        }
        tok = scan();
      /* ... */
      }
      
    }

    /* ********** Functions ********** */
    else if(tok == '(') /* function */
    {
      /* [ name { , name } ] ) statement */
      
//      printf("Function ");
      if(inFunction)       /* A function inside another is not allowed */
      {
        rtn = -1;
        /* ERROR */
      }
      inFunction = 1;
      tok = scan();
      while(tok == TOK_IDENT)
      {
        /* process name */
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
//        printf("Look for statement...\n");
        statement();
      }
      else
      {
        /* error */
        rtn = -1;
      }
      inFunction = 0;     /* Tell the world we are done */
//      printf("Exiting Function \n");
    }

    /* ********** Initialized Variable ********** */
    else if(tok == TOK_INT || tok == TOK_IDENT || tok == TOK_STRING)
    {
//      printf( "Initalized simple variable \n");
      tok = scan();
      if(tok != ';')
      {
        printf("Error: expected ; \n");
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
//      printf("Simple Variable \n");
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
    exit(1);
  }

    

//  printf("Exiting Definition \n");
  return rtn;
}

/* ***********************************************************************
 * @fn lval
 * @brief parses an lvalue
 * @return parse status
 * ******************************************************************** */
//word lval()
//{
//  word rtn = 0;
//  /* TODO: fill this in */

//  return rtn;
//}

/* ***********************************************************************
 * @fn statement
 * @brief parses a statement
 * @return parse status
 * ******************************************************************** */
word statement()
{
  word rtn = 0;

  /* TODO: fill this in */
//  printf("In statement with token: %d\n", tok);
  while(tok == TOK_AUTO || tok == TOK_EXTRN
        || tok == TOK_CASE ) // TODO labels || tok == TOK_IDENT)
  {
    printf("Statement tok = %d \n", tok);
    switch(tok)
    {
    case TOK_AUTO:                         /* keyword auto */
      tok = scan();
//      printf("auto definition\n");
      if(tok != TOK_IDENT)
      {
        /* error */
        rtn = -1;
        tok = scan(); /* what now? */
      }
      else                                 /* got name */
      {
        /* put into table, on stack */
        tok = scan();
        if(tok == TOK_INT || tok == TOK_STRING || tok == TOK_IDENT)  /* initializer? */
        {
          printf("Found initializer of type %d\n", tok);
          /* TODO initialize */
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
          }
          else
          {
//            printf("Found semicolon at end of auto init \n");
            //while(1);
            tok = scan();
          }
        }
        else if(tok == ';')  /* not initialized, look for terminator */
        {
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
      }
      else
      {
        /* TODO put into table */
        tok = scan();
        if(tok != ';')
        {
          /* error */
          /* TODO what else here? */
          rtn = -1;
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
//    printf("IF STATEMENT \n");
    tok = scan();
    if(tok != '(')
    {
      /* error */
      rtn = -1;
      break;
    }
    tok = scan();
//    printf("IF STATEMENT -- entering expression\n");
    expression();
//    printf("IF STATEMENT -- exited expression\n");
    if(tok != ')')
    {
      /* error */
      printf("IF ST ERROR no ) found %d \n", tok);
      rtn = -1;
      break;
    }
    printf("\tJZ\tELSEPART\n");
    tok = scan();
//    printf("IF STATEMENT -- entering statement\n");
    statement();
    printf("\tJMP\tENDIF\n");
    printf("ELSEPART\n");
    if(tok == TOK_ELSE)
    {
      tok = scan();
      statement();
    }
    printf("ENDIF\n");
    /* todo */
    break;
    
  case TOK_WHILE:  /* while '(' rvalue ')' statement */
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
//    printf("goto st\n");
    tok = scan();
    /* todo */
    break;
  case TOK_RETURN: /* return [ (rvalue) ] ; */
//    printf("return st\n");
    tok = scan();
    if(tok != ';')
    {
      /* ERROR */
      /* TODO need an expression parse here */
      expression();
    }
    if(tok != ';')
    {
      /* ERROR  expected ; */
      rtn = -1;
    }
    tok = scan();
    break;
  case '{':        /* '{' {statement} '}' */
    //printf("Compound st\n");
    tok = scan();
    while(tok != '}')
    {
      //printf("In compound statement this tok = %d\n", tok);
      statement();
      //printf("tok after statement = %d\n", tok);
    }
    //printf("Compound statement ended this tok = %d\n",tok);
    tok = scan();
    //printf("...and next tok = %d\n", tok);
    break;

    
    //statement();
    //while(tok == ';')
    //{
    //  tok = scan();
    //  if(tok == '}')
    //  {
    //    printf("Compound Statement ended \n");
    //    tok = scan();
    //    break;
    //  }
    //  else
    //  {
    //    printf("Comp ST cont ...");
    //    statement();
    //  }
    //}
    /* todo */
    //printf("Exiting Compount \n");
    //break;
    
  default:         /* [rvalue] ; */
    //printf("EXPRESSION STATEMENT \n");
    expression();
    if(tok == ';')                  /* Good, found the end */
    {
      tok = scan();
      //printf("End of EXP st next tok=%d\n", tok);
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
//  printf("EXPRESSION \n");
  assignexp();

  /* TODO: */

  return rtn;
}

/* ***********************************************************************
 * @fn rvalue
 * @brief parses an rvalue
 * @return parse status
 * ******************************************************************** */
/* rvalue = ( rvalue )
          | lvalue               ( name, *rvalue, rvalue [ rvalue ] )
          | constant             ( int, string )
          | lvalue assign rvalue
          | inc-dec lvalue
          | lvalue inc-dec
          | unary rvalue
          | & lvalue
          | rvalue binary rvalue
          | rvalue ? rvalue | rvalue
          | rvalue ( {rvalue {, rvalue} } )
*/

//word rvalue()
//{
//  word rtn = 0;

//  /* TODO: */

//  return rtn;
//}

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
  //printf("ASSIGN EXPRESSION\n");
  conditionalexp();
  while( isassignop(tok))
  {
    /* TODO */
    thisTok = tok;
    //printf("Found ASSIGNOP tok=%d\n",tok);
    tok = scan();
    assignexp();
    printf("\tDOASSIGN_%03d\n",thisTok);
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
  //printf("CONDITIONAL EXPRESSION\n");

  rtn = orexp();
  if(tok == '?')
  {
    printf("\tJMZ\tFALSEPART\t\t;If cond exp was false, jump over true part\n");
    tok = scan();
    rtn = orexp();
    trueValue = valueType;  /* Get true part value */
    printf("FALSEPART: \n");
    if(tok == ':')
    {
      tok = scan();
      rtn = orexp();
      falseValue = valueType; /* Get false part value */
      printf("\tJMP\tENDOFCOND\n");
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
    printf("ENDOFCOND\n");
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
  //printf("OR EXPRESSION\n");

  rtn = andexp();
  while(tok == '|')
  {
    valueType = RVALUE;
    tok = scan();
    rtn = andexp();
    printf("\tOR \n");
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
  //printf("AND EXPRESSION\n");

  rtn = equalexp();
  while(tok == '&')
  {
    valueType = RVALUE;
    tok = scan();
    rtn = equalexp();
    printf("\tAND \n");
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
  //printf("EQUAL EXPRESSION\n");
  word thisTok;

  rtn = relateexp();
  while(tok == TOK_EQ_EQ | tok == TOK_NOT_EQ)
  {
    thisTok = tok;
    valueType = RVALUE;
    tok = scan();
    rtn = relateexp();
    if(thisTok == TOK_EQ_EQ)
    {
      printf("\tCMPEQ \n");
    }
    else
    {
      printf("\tCMPNEQ \n");
    }
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
  //printf("RELATE EXPRESSION\n");
  word thisTok;

  rtn = shiftexp();
  while( tok == '<' | tok == '>'
         | tok == TOK_LESS_EQ | tok == TOK_GREAT_EQ )
  {
    thisTok = tok;
    valueType = RVALUE;
    tok = scan();
    rtn = shiftexp();
    switch(thisTok)
    {
    case '<':
      printf("\tCMPLESS \n");
      break;
    case '>':
      printf("\tCMPGRT \n");
      break;
    case TOK_LESS_EQ:
      printf("\tCMPLEQ \n");
      break;
    case TOK_GREAT_EQ:
      printf("\tCMPGEQ \n");
      break;
    default:
      /* ERROR */
      break;
    }
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
  //printf("SHIFT EXPRESSION\n");
  word thisTok;

  rtn = addexp();
  while(tok == TOK_SHIFT_LEFT | tok == TOK_SHIFT_RIGHT)
  {
    thisTok = tok;
    valueType = RVALUE;
    tok = scan();
    rtn = addexp();
    if(thisTok == TOK_SHIFT_LEFT)
    {
      printf("\tSHL \n");
    }
    else
    {
      printf("\tSHR \n");
    }
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
  //printf("ADD EXPRESSION\n");
  word thisTok;

  rtn = term();
  while(tok == '+' | tok == '-')
  {
    thisTok = tok;
    valueType = RVALUE;
    tok = scan();
    rtn = term();
    printf("\tADD \n");
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
  //printf("TERM\n");
  word thisTok;

  rtn = factor();
  while(tok == '*' | tok == '/' | tok == '%')
  {
    thisTok = tok;
    //printf("term: thisTok = %d\n", thisTok);
    valueType = RVALUE;
    tok = scan();
    rtn = factor();
    switch(thisTok)
    {
    case '*':
      printf("\tMUL \n");
      break;
    case '/':
      printf("\tDIV \n");
      break;
    case '%':
      printf("\tMOD \n");
      break;
    default:
      /* ERROR */
      break;
    }
    
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
  rtn = 0;
  //printf("FACTOR\n");
  word val;
  // TODO word tokChars;
  char* tokChars;
  
  /* TODO */
  switch(tok)
  {
  case TOK_INT:           /* dec, oct, char, : rvalue */
    valueType = RVALUE;
    val = getNumber();
    tok = scan();
    if(tok == '[')        /* vector */
    {
      valueType = LVALUE;
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
    else if(tok == '(')   /* function call */
    {
      valueType = RVALUE;
      tok = scan();
      /* paramlist */
      /* ... */
      /* TODO */
      printf("\tCALL\t%04d\n",val);
    }
    else   /* just a number */
    {
      printf("\tPUSH\t%04d\n", val);
    }
    
    break;
    
  case TOK_STRING:        /* "cccc..."   :rvalue */
    valueType = RVALUE;
    val = getNumber();
    tok = scan();
    printf("\tPUSH\t%04d\t\t;Push string address\n",val);
    break;
    
  case TOK_IDENT:         /* name */
    valueType = LVALUE;
    tokChars = getText();
    /* TODO convert this to B */
    char localText[128];
    strcpy(localText,tokChars);
    //printf("Got identifier: %s\n",localText);
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
      valueType = RVALUE;
      tok = scan();
      /* paramlist */
      /* ... */
      /* TODO */
    }
    else /* just a plain variable */
    {
      // TODO val = lookup(getText);
      printf("\tPUSH\t-%s-\t\t;Get a variable address \n",localText);
    }
    
    break;

  case '-':                /* negation  */
    tok = scan();
    factor();
    printf("\tNEG \n");
    break;

  case '!':                /* logical NOT */
    tok = scan();
    factor();
    printf("\tNOT\n");
    break;

  case '~':                /* bitwise NOT */
    tok = scan();
    factor();
    printf("\tCPL\n");
    break;

  case '&':                /* address of */
    tok = scan();
    factor();
    printf("\tADDRESS OF \n");
    /* TODO hmmmmm */
    break;

  case '*':                /* indirection */
    tok = scan();
    factor();
    printf("\tFETCH\n");
    break;

  default:
    rtn = -1;
    /* error */
  }
  
    
  return(rtn);
}


int main(int argc, char** argv)
{
  int rtn = 0;
  int infile = -1;
  if(argc > 1)
  {
    if(! scanfile(argv[1]))
    {
      printf("Parsing file %s ... \n", argv[1]);
      parse( 0 );
      printf("Done.\n");
    }
    else
    {
      printf("Couldn't open %s \n", argv[1]);
      rtn = 1;
    }
    
    /*********************
    infile = open(argv[1], O_RDONLY);
    if(infile < 0)
    {
      printf("Could not open file %s, exiting \n", argv[1]);
      exit(1);
    }
    printf("Parsing %s ... \n", argv[1]);
    parse(infile);
    printf("Done.\n");
    *************************/
  }
  else
  {
    printf("No input file specified, exiting. \n");
      exit(1);
  }
  return 0;
}


  
    

