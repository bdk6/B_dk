
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "definitions.h"
#include "parser.h"
#include "scanner.h"

// typedef int16_t word;

word tok;

#define UNVALUE   0
#define RVALUE    1
#define LVALUE    2

word valueType = UNVALUE;

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
  if(tok == TOK_IDENT) /* name */
  {
    /* TODO: ... */
    printf("Name ");
    tok = scan();
    if(tok == '[')    /* vector */
    {
      /* vector */
      /* constant ] ival {, ival } */
      printf("Vector ");
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
        while(tok == TOK_INT || tok == TOK_IDENT)
        {
          /* ... */
          /* process constant or name */
          tok = scan();
          if(tok != ',')
          {
            break;
          }
        }
      /* ... */
      }
      
    }
    else if(tok == '(') /* function */
    {
      /* [ name { , name } ] ) statement */
      printf("Function ");
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
        printf("Look for statement...\n");
        statement();
      }
      else
      {
        /* error */
        rtn = -1;
      }
    }
  }
  else /* error: expected a name to define */
  {
    /* error routine */
    printf("Definition Error - expected name to define \n");
    while(1);
    rtn = -1;
  }

    

  return rtn;
}

/* ***********************************************************************
 * @fn lval
 * @brief parses an lvalue
 * @return parse status
 * ******************************************************************** */
word lval()
{
  word rtn = 0;
  /* TODO: fill this in */

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

  /* TODO: fill this in */
  printf("In statement with token: %d\n", tok);
  while(tok == TOK_AUTO || tok == TOK_EXTRN
        || tok == TOK_CASE || tok == TOK_IDENT)
  {
    printf("Statement ");
    switch(tok)
    {
    case TOK_AUTO:
      tok = scan();
      /* todo */
      break;
    case TOK_EXTRN:
      tok = scan();
      /* todo */
      break;
    case TOK_CASE:
      tok = scan();
      /* todo */
      break;
    case TOK_IDENT:
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
      rtn = -1;
      break;
    }
    tok = scan();
    statement();
    if(tok == TOK_ELSE)
    {
      tok = scan();
      statement();
    }
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
    printf("goto st\n");
    tok = scan();
    /* todo */
    break;
  case TOK_RETURN: /* return [ (rvalue) ] ; */
    printf("return st\n");
    tok = scan();
    /* todo */
    break;
  case '{':        /* '{' {statement} '}' */
    printf("Compound st\n");
    tok = scan();
    /* todo */
    break;
    
  default:         /* [rvalue] ; */
    
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

word rvalue()
{
  word rtn = 0;

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
  if( isassignop(tok))
  {
    /* TODO */
    thisTok = tok;
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

  rtn = orexp();
  if(tok == '?')
  {
    tok = scan();
    rtn = orexp();
    trueValue = valueType;  /* Get true part value */
    if(tok == ':')
    {
      tok = scan();
      rtn = orexp();
      falseValue = valueType; /* Get false part value */
    }
    else
    {
      /* error */
    }

    /* ???? more? */
    if(trueValue == LVALUE && falseValue == LVALUE)
    {
      valueType = LVALUE; /* defaulted to RVALUE */
    }
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

  rtn = relateexp();
  while(tok == TOK_EQ_EQ | tok == TOK_NOT_EQ)
  {
    valueType = RVALUE;
    tok = scan();
    rtn = relateexp();
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

  rtn = shiftexp();
  while( tok == '<' | tok == '>'
         | tok == TOK_LESS_EQ | tok == TOK_GREAT_EQ )
  {
    valueType = RVALUE;
    tok = scan();
    rtn = shiftexp();
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

  rtn = addexp();
  while(tok == TOK_SHIFT_LEFT | tok == TOK_SHIFT_RIGHT)
  {
    valueType = RVALUE;
    tok = scan();
    rtn = addexp();
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

  rtn = term();
  while(tok == '+' | tok == '-')
  {
    valueType = RVALUE;
    tok = scan();
    rtn = term();
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

  rtn = factor();
  while(tok == '*' | tok == '/' | tok == '%')
  {
    valueType = RVALUE;
    tok = scan();
    rtn = factor();
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

  /* TODO */
  switch(tok)
  {
  case TOK_INT:           /* dec, oct, char, : rvalue */
    valueType = RVALUE;
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
    }
    
    break;
    
  case TOK_STRING:        /* "cccc..."   :rvalue */
    valueType = RVALUE;
    tok = scan();
    break;
    
  case TOK_IDENT:         /* name */
    valueType = LVALUE;
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


  
    

