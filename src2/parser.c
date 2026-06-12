/* B parser */
/* Copyright William Cooke 2024 */

//#include <stdint.h>
#include <stdio.h>

#include "definitions.h"
#include "scanner.h"
#include "symtable.h"
#include "codegen.h"

// typedef int16_t word;


word tok;

word p_definition(void);
word p_ival(void);
word p_statement(void);

extern word TOK_PLPL;
extern word TOK_MINMIN;
extern word TOK_SHLFT;
extern word TOK_SHRT;

extern word TOK_EOF;
extern word TOK_IDENT;
extern word TOK_INT;
extern word TOK_STRING;


extern word TOK_AUTO;
extern word TOK_EXTRN;
extern word TOK_CASE;
extern word TOK_IF;
extern word TOK_GOTO;
extern word TOK_RETURN;
extern word TOK_SWITCH;
extern word TOK_WHILE;

/* functions */
// BDK extern word tok_string(void);
extern word findSym(word* name, word isLocal);


/* forward declarations*/
word p_expression(void);
word p_assign_exp(void);
word cond_exp(void);
word logOR_exp(void);
word logAND_exp(void);
word OR_exp(void);
word XOR_exp(void);
word AND_exp(void);
word equal_exp(void);
word relat_exp(void);
word sh_exp(void);
word add_exp(void);
word mul_exp(void);
word un_exp(void);
word prm_exp(void);
word post_exp(void);



/* parse stack operation */
word lvalue = 0;
word rvalue = 1;
word p_stack[100];  /* parse stack */
word p_stkp = 0;    /* parse stack pointer */
word parmoffs = 0;  /* parameter offset */
word autooffs = 0; /* offset of previous local on stack */

/* fn call stack
   parm n:    n+3
   parm n-1:  n-1+3
   ...
   parm 0:     3
   parmcnt:    2
   retval:     1
   retaddr:    0  <-- frame ptr
   auto 0:    -1
   auto 1     -2 
   auto n:    -n-1
   */

word in_fn   = 0;   /* true if in fn definition */
//word auto_idx = 0;  /* number of next auto variable*/
word p_push(word i)
{
  word rtn = p_stkp;
  p_stack[p_stkp++] = i;
  return rtn;
}

word p_pop(void)
{
  word rtn = p_stack[--p_stkp];
  return rtn;
}

word p_peek(void)
{
  return p_stack[p_stkp - 1];
}


/* Parse Errors */
word pe_expid = 1;          /* Expected Identifier */

word parse_err(word err)
{
  word rtn = 0;
  printf("\nParse error %d on line %d with tok %d\n", err, line_num, tok);
while(1);

  return rtn;
}

/* Main parse entry point */
word p_program(void)
{
  word rtn = 0;
  tok = scan();
  while(tok != TOK_EOF)
  {
    p_definition();
  }

  return rtn;
}

word p_definition(void)
{
  word rtn = 0;
  if( tok != TOK_IDENT)
  {
    //parse_err(pe_expid);
    printf("expected ID got %d\n", tok);
    parse_err(pe_expid); // 1
    while(1);
  }
  else
  {
    /* TODO insert in table */
    // TODO word idx = SYM_INSERT();
    tok = scan();
    /* function */
    if( tok == '(' )
    { printf("fn def\n");
      in_fn = 1;  /* tell the world we are defining a fn */
      parmoffs = 0;   /* set offset for next parameter */
      autooffs = 3;   // set offset for next local var
      /* TODO parse function definition*/
      /* parameters */
      word parmcnt = 0;
      tok = scan();
      while( tok == TOK_IDENT)
      {
        /* enter into symtable with auto, offset = parmcnt + 4 */
        parmcnt++;
        tok = scan();
        if(tok == ',')
        {
          tok = scan();  /* ident or ')' */
        }
      }
      /* right paren*/
      if(tok != ')' ) parse_err(2);

      /* header -- make frameptr */
      rtn = gen_sfp();
      word locals = 0;
      tok = scan();
      /* statement */
      p_statement();

      // TODO generate return if not done
      in_fn = 0; /* done now, tell the world */
      printf("ended function\n");
    }

    /* variables */
    else
    { printf("var def\n");
      if(tok == '[')
      {
        /* vector*/
        tok = scan();
        /* TODO get optional constant */
        if( tok == TOK_INT ) 
        {
          // TODO set size, scan
          printf("parse vector size tok: %d\n", tok);
          tok = scan();
        }
        if( tok != ']')
        {
          parse_err( 3 ); // TODO error numbers
        }
        tok = scan();

      }
      /* initializer list */
      while( tok == TOK_INT )
      { printf("initializer, ");
        // TODO initialize this value
        tok = scan();
        if( tok != ',')
        {
          break;
        }
        tok = scan();
      } printf("end \n");

      if(tok != ';')
      {
        parse_err(4);
      }
      tok = scan();
    }
  }

  return rtn;
}

/* TODO see if we need this */
/* p_ival parse initial values */
word p_ival(void)
{
  word rtn = 0;

  return rtn;
}

/* parameter list */
word parmlist(void)
{
  word rtn = 0;

  return rtn;
}

/* p_statement */
word p_statement(void)
{ printf("statement --- ");
  word rtn = 0;
  
  /* TODO case and label and */
  if(tok == '{')
  { printf("compound\n");
    /* compound st */
 
    tok = scan();
    while(tok != '}')
    { printf("compound tok = %d", tok);
      rtn = p_statement();
      // done  for each statement if(tok != ';') parse_err(5);
      //tok = scan();

    }
  }

  /* auto var */
  else if (tok == TOK_AUTO )
  { printf("auto decl\n");
    // TODO check if in fn
    if(!in_fn) parse_err(6);
    // 
    tok = scan();
    while(tok == TOK_IDENT)
    { printf("ident auto var\n");
      // TODO insert symbol (create offset)
      word idx = insertSym(&tok, 1);  // insert name as local
      symsetadr(idx, autooffs++); /* Set var offset from FP */
      tok = scan();
      if(tok == TOK_INT)
      {
        gen_auto(0 /* constant */);
        tok = scan();
      }
      if(tok == ',') tok = scan(); /* another... */
      /* if not a comma, should be a ";" */
    }
    // once at end if(tok != ';') parse_err(7);
    // TODO should be a statement now

  }

  /* extrn var */
  else if (tok == TOK_EXTRN)
  { printf("extrn decl\n");

  }

  // /* labeled statement */
  // else if(tok == TOK_IDENT)
  // { ***************   could be expression ***********
  //   // TODO Get current address
  //   // TODO enter symbol
  //   // TODO align word
  //   tok = scan();
  //   if(tok != ':') parse_err(8);
  //   tok = scan();
  //   rtn = p_statement();

  // }

  /* case label */
  else if(tok == TOK_CASE)
  { printf("case statement\n");

  }

  /* if statement */
  else if(tok == TOK_IF)
  { printf("if statement\n");
    tok = scan();
    if(tok != '(') parse_err(9);
    tok = scan();
    rtn = p_expression();
    if(tok != ')') parse_err(10);
    tok = scan();
    gen_jz(0); // TODO and save address
    rtn = p_statement();
    if(tok == TOK_ELSE)
    {
      // TODO patch jz above
      gen_jmp(0); // TODO and save address
      rtn = p_statement();
      // TODO patch jmp above
    }
    else
    {
      // TODO patch jz above
    }
    // once at end if(tok != ';') parse_err(11);

  }

  /* while statement */
  else if(tok == TOK_WHILE)
  { printf("while statement\n");
    tok = scan();
    if(tok != '(')
    {
      parse_err(12);
    }
    tok = scan();
    // TODO save this address to jump back to
    rtn = p_expression();
    gen_jz(0);
    rtn = p_statement();
    gen_jmp(0); // TODO the saved address above
    // TODO patch jz above with this address

  }

  /* switch statement */
  else if(tok == TOK_SWITCH)
  { printf("switch statement\n");
    // TODO might need some more code here :-)

  }

  /* goto statement */
  else if(tok == TOK_GOTO)
  { printf("goto statement\n");
     tok = scan();
     rtn = p_expression();
     gen_jsk();  // jmp to value on stack
     // once at end if(tok != ';' ) parse_err(13);
  }

  /* return statement */
  else if(tok == TOK_RETURN)
  { printf("return statement\n");
    tok = scan();  // Is there an expression?
    if(tok == '(' )
    {
      rtn = p_expression();
      // TODO gen_save_return_value();
    }
    gen_rtn();
    // once at end if(tok != ';') parse_err(14);
  }

  /* expression statement */
  else
  { printf("expression statement\n");
    tok = scan();
    rtn = p_expression();
    gen_drp();   /* drop the value */
    // once at end if(tok != ';' ) {printf("tok: %d ", tok); parse_err(15);}
    // too far tok = scan();
  }

  if(tok != ';') parse_err(5);
  tok = scan();
  return rtn;
}
/* expressions (mod from K&R 2e )*/

/* expression */
/* expression ::= assign-expr { ',' assign-expr}. but no comma op in B */
word p_expression(void)
{ printf("expression\n");
  word rtn = 0;
  rtn = p_assign_exp();
  return rtn;
}

/* return true if current token is unary operator */
/* ++ -- + - * & ! */
word isunop(void)
{
  word rtn = 0;
  if(tok == TOK_PLPL || tok == TOK_MINMIN || tok == '+' || tok == '-' 
     || tok == '*' || tok == '&' || tok == '!')
     {
      rtn = 1;
     }

  return rtn;
}

/* return true if current tok is assignment operator*/
word isassop(void)
{
  word rtn = 0;
  if(tok == '=' || tok == TOK_EQPLUS || tok == TOK_EQAND || tok == TOK_EQMIN
     || tok == TOK_EQMOD || tok == TOK_EQOR || tok == TOK_EQPLUS 
     || tok == TOK_EQSHL || tok == TOK_EQSHR || tok == TOK_EQSLSH 
     || tok == TOK_EQSTAR || tok == TOK_EQXOR)
  {
    rtn = 1;
  }
printf("isassop: %d\n", rtn);
  return rtn;
}

/* assign-exp */
/* conditional-exp | unary-exp assign-op assign-exp */
// BDK TODO PROBLEM HERE -- goes to cond exp if no unary op
word p_assign_exp(void)
{ printf("assign exp\n");
  word rtn = 0;
  word op = 0;

// TODO fill this in

// TODO if(isunop()) /* unary-exp assign-op assign-exp */
{ printf("assignment\n");
  rtn = logOR_exp(); // TODO un_exp();
  if(isassop())
  { printf("assign op %d\n", tok);
    op = tok;
    tok = scan();
    // TODO make the assignment
    rtn = p_assign_exp();
    printf("make the assignment\n");

  }
}

// else /* conditional-exp */
// {printf("conditional\n");
//   rtn = cond_exp();
// }
  return rtn;
}

/* conditional expression */
/* cond-expr ::= logOR_exp [ '?' expression ':' cond-exp ]*/
word cond_exp(void)
{ printf("conditional exp\n");
  word rtn = 0;

  // logor exp
  rtn = logOR_exp();
  if(tok == '?')
  {printf("cond op %c\n", tok);
    gen_jz(0);
    tok = scan();
    rtn = p_expression();
    gen_jmp(0);
    // TODO patch jz with this address
    if(tok != ':') parse_err(16);
    rtn = cond_exp();
    // TODO patch jmp with this address
    return rtn;
  }
  

  return rtn;
}

/* logOR_exp -- extension */
/* logOR_exp ::= logAND_exp { "||" logAND_exp }*/
word logOR_exp(void)
{ printf("logical or exp\n");
  return logAND_exp();
  //word rtn = 0;
  //rtn = logAND_exp();
  //return rtn;
}

/* logAND_exp -- extension */
/* logAND_exp ::= incOR_exp { "&&" incOR_exp }*/
word logAND_exp(void)
{  printf("logical and exp\n");
  return OR_exp();

  //word rtn = 0;
  //rtn = incOR_exp();
  //return rtn;
}

/* incOR_exp */
/* incOR_exp ::= excOR_exp { "|" excOR_exp }*/
word OR_exp(void)
{ printf("OR exp\n");
  word rtn = 0;
  word ltyp = 0;
  word rtyp = 0;

  rtn = XOR_exp();
  while(tok == '|')
  {  printf("or op %c\n", tok);// TODO scan?
    ltyp = p_pop();
    if(ltyp == lvalue) gen_fetch(); /* now its an rvalue */
    
    rtn = XOR_exp();
    rtyp = p_pop();
    if(rtyp == lvalue) gen_fetch(); /* now its an rvalue */
    gen_or();
    p_push(rvalue);
  }

  return rtn;
}

/* excOR_exp */
/* excOR_exp ::= AND_exp { "^" AND_exp} */
word XOR_exp(void)
{ printf("XOR exp\n");
  word rtn = 0;
  word ltyp = 0;
  word rtyp = 0;
  // TODO !!! UGH!!! I've been popping too many!

  rtn = AND_exp();
  while(tok == '^')
  {  printf("xor op %c\n", tok);// TODO scan?
    ltyp = p_pop();
    if(ltyp == lvalue) gen_fetch(); /* now its an rvalue */
    
    rtn = AND_exp();
    rtyp = p_pop();
    if(rtyp == lvalue) gen_fetch(); /* now its an rvalue */
    gen_xor();
    p_push(rvalue);
  }

  return rtn;
}

/* AND_exp */
/* AND_exp :::= equal_exp { "&" equal_exp }*/
word AND_exp(void)
{ printf("AND exp\n");
  word rtn = 0;
  word ltyp = 0;
  word rtyp = 0;

  rtn = equal_exp();
  while(tok == '&')
  { printf("and op %c\n", tok); // TODO scan?
    ltyp = p_pop();
    if(ltyp == lvalue) gen_fetch(); /* now its an rvalue */
    // TODO not needed word op = tok;
    rtn = equal_exp();
    rtyp = p_pop();
    if(rtyp == lvalue) gen_fetch(); /* now it an rvalue */
    gen_and();
    p_push(rvalue);
  }

  return rtn;
}

word iseqop(word tok)
{
  word rtn = 0;
  if(tok == TOK_EQEQ || tok == TOK_NOTEQ) rtn = 1;
  return rtn;
}

/* equal_exp */
/* equal_exp ::= relat_exp { eq_op relat_exp } */
/* eqop ::= ==, != */
word equal_exp(void)
{ printf("equal exp\n");
  word rtn = 0;
  word ltyp = 0;
  word rtyp = 0;

  rtn = relat_exp();
  while( iseqop(tok))
  {  printf("equal op %d\n", tok); // TODO scan?
    ltyp = p_pop();
    if(ltyp == lvalue) gen_fetch(); /* now its an rvalue*/
    word op = tok;
    rtn = relat_exp();
    rtyp = p_pop();
    if(rtyp == lvalue) gen_fetch(); /* now its an rvalue */
    if(op == TOK_EQEQ) gen_equal();
    else if(op = TOK_NOTEQ) gen_noteq();
    else parse_err(17);
    p_push(rvalue);
  }
  return rtn;
}

word isrelop(word tok)
{
  word rtn = 0;
  if(tok == '<' || tok == '>' || tok == TOK_LESSEQ || tok == TOK_GRTEQ)
    rtn = 1;
  return rtn;
}

/* relat_exp */
/* relat_exp ::= sh_exp { relop sh_exp } */
/* relop ::= <, >, <=, >= */
word relat_exp(void)
{ printf("relational exp\n");
  word rtn = 0;
  word ltyp = 0;
  word rtyp = 0;

  rtn = sh_exp();
  while( isrelop(tok))
  { // TODO scan?
    ltyp = p_pop();
    if(ltyp == lvalue) gen_fetch(); /* now its an rvalue */
    word op = tok;
    rtn = sh_exp();
    rtyp = p_pop();
    if(rtyp == lvalue) gen_fetch(); /* now its an rvalue */
    if(op == '<') gen_less();
    else if(op == '>') gen_grtr();
    else if(op == TOK_LESSEQ) gen_lesseq();
    else if(op == TOK_GRTEQ) gen_grteq();
    else parse_err(18);
    p_push(rvalue);
  }
  return rtn;
}

word isshop(word tok)
{
  word rtn = 0;
  if(tok == TOK_SHLFT || tok == TOK_SHRT) 
  rtn = 1;

  return rtn;
}

/* sh_exp */
/* sh_exp ::= add_exp { shop add_exp } */
/* shfop ::= <<, >> */
word sh_exp(void)
{ printf("shift exp\n");
  word rtn = 0;
  word ltyp = 0;
  word rtyp = 0;

  rtn = add_exp();
  while(isshop(tok))
  { // TODO scan?
    ltyp = p_pop();
    if(ltyp == lvalue) gen_fetch(); /* now its an rvalue */
    word op = tok;
    rtn = add_exp();
    rtyp = p_pop();
    if(rtyp == lvalue) gen_fetch(); /* now its an rvalue */
    if( op == TOK_SHLFT) gen_shl();
    else if( op == TOK_SHRT ) gen_shr();
    else parse_err(19);
    p_push(rvalue);
  }
  return rtn;
}

word isaddop(word tok)
{
  word rtn = 0;
  if(tok == '+' || tok == '-') rtn = 1;
  return rtn;
}

/* add_exp */
/* add_exp ::= mul_exp { addop mul_exp }*/
/* addop ::= +,- */
word add_exp(void)
{ printf("add exp\n");
  word rtn = 0;
  word ltyp = 0;
  word rtyp = 0;

  rtn = mul_exp();
  while(isaddop(tok))
  {  printf("addop %c\n", tok); // TODO scan?
    ltyp = p_pop();
    if(ltyp == lvalue) gen_fetch(); /* now its an rvalue */
    word op = tok;
    tok = scan();
    rtn = mul_exp();
    rtyp = p_pop();
    if(rtyp == lvalue) gen_fetch(); /* now its an rvalue */
    
    if(op == '+') gen_add();
    else if(op == '-' ) gen_sub();
    else parse_err(20);
    p_push(rvalue);
  }
  return rtn;
}

word ismulop(word tok)
{
  word rtn = 0;
  if(tok == '*' || tok == '/' || tok == '%') rtn = 1;
  printf("ismulop tok: %d\n", tok);
  return rtn;
}

/* mul_exp */
/* mul_exp ::= un_exp { mulop un_exp }*/
/* mulop ::= *, /, % */
word mul_exp(void)
{ printf("mul exp\n");
  word rtn = 0;
  word ltyp = 0;
  word rtyp = 0;

  rtn = un_exp(); printf("done with un_exp tok %d\n", tok);
  while(ismulop(tok))
  {  printf("mulop %c\n", tok); // TODO scan?
    ltyp = p_pop();
    if(ltyp == lvalue) gen_fetch(); /* now its an rvalue */
    word op = tok;
    rtn = un_exp();
    rtyp = p_pop();
    if(rtyp == lvalue) gen_fetch(); /* now its an rvalue */

    if(op == '*') gen_mul();
    else if(op == '/') gen_div();
    else if(op == '%') gen_mod();
    else parse_err(21);
    p_push(rvalue);
  }
  return rtn;
}


/* un_exp */
/* un_exp ::= postfix_exp | '++' un_exp | '--' un_exp | unop un_exp */
/* unop ::= & * + - ~ ! */
word un_exp(void)
{ printf("unary exp\n");
  word rtn = 0;
  
  switch(tok)
  {
    case 286: // TOK_PLPL:
    // TODO
      break;
    case 287: // TOK_MINMIN:
      // combine inc and dec ? word thisop = tok;
      tok = scan();
      un_exp();
      if( p_peek() == lvalue)
      {
        // dec rvalue, store it, and leave it
        gen_dup();
        gen_dup();  // get two copies
        // TODO finish this
      }
      else
      {
        // parse error
        parse_err(22);
      }
      break;
    case '&':
      tok = scan();
      un_exp();
      if(p_peek() != lvalue)
      {
        // parse error
        parse_err(23);
      }
      p_pop(); 
      p_push(rvalue);
      break;
    case '*':
      tok = scan();
      un_exp();
      gen_fetch();  /* get the value pointed to */
      if(p_peek() == lvalue) /* make sure we change to rvalue */
      {
        p_pop();
        p_push(rvalue);
      }

      break;
    // case '+': // TODO remove?
    //   break;
    case '-':
      tok = scan();
      un_exp();
      if(p_peek() == lvalue)
      {
        gen_fetch();
        p_pop();
        p_push(rvalue);
      }
      gen_neg();
      break;
    // case '~':  // TODO remove?
    //  break;
    case '!':
      tok = scan();
      un_exp();
      if(p_peek() == lvalue)
      {
        gen_fetch();
        p_pop();
        p_push(rvalue);
      }
      gen_not();
      break;
    default:  /* postfix exp */
      rtn = post_exp();
      break;
  }
  return rtn;
}

/* post_exp */
/* post_exp ::= prm_exp { postop }*/
/* postop ::= [exp], (argexplist), ++, -- */
word post_exp(void)
{ printf("post exp\n");
  word rtn = 0;
  word parms = 0;  /* number of function parameters */
  rtn = prm_exp();
  switch(tok)
  {
    case '[':
      tok = scan();
      rtn = p_expression();
      // TODO code here
      if(tok != ']') parse_err(25);
      tok = scan();
      break;

    case '(':
      tok = scan();
      while(tok != ')')
      {
        parms++;
        rtn = p_expression();
        if(tok != ',' || tok != ')') parse_err(26);
        tok = scan();
      }
      tok = scan();
      break;

    case 286: // TOK_PLPL:
      break;
    case 287: // TOK_MINMIN:
      break;
    default:
      break;

  }
  return rtn;
}

/* primary expression */
/* prm_exp ::= ident | const | string | '(' expression ')' */
word prm_exp(void)
{ printf("primary exp\n");
  word rtn = 0;
  switch(tok)
  {
    case 256: // TOK_IDENT:  /* var, vector, fn, label, ???  -- extern*/
      // push lvalue (address of item )
      // Get lvalue from symtable and push
      printf("ident\n");
      p_push(lvalue);
      tok = scan();
      if( tok == '(')
      { printf("primary -- fn call\n");
        while((tok = scan()) != ')');
        tok = scan();
      }
      break;
    case 257: // TOK_INT:  // constant
    {
      // push rvalue
      // get rvalue
      printf("int\n");
      word x = 0;
      gen_pushConstant(x);
      p_push(rvalue);
      tok = scan();
      break;
    } 
    case 258: // TOK_STRING:
    { // push rvalue (address)?
      printf("string\n");
      word addr = gen_string(0);
      gen_pushConstant(addr);
      p_push(rvalue);
      tok = scan();
      break;
    }
    case '(':
    // scan
    // expression
      break;
    default:
    // syntax error
      parse_err(24);
      break;

  }
}

/* constant ::= integer-constant | character-constant | floating-constant | enumeration-constant */

int main(int argc, char* argv[])
{
  int rtn = 0;
  rtn = p_program();


  return rtn;
}
