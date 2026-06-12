/* ***** scanner.c ******************* */
/* ***** copyright 2024 William R Cooke */

#include <stdio.h>
#include "definitions.h"


/* Here are the token types.                 */
/* The single character tokens simply have   */
/* their ascii value.  Below are the values  */
/* for all multi-character token types.      */
word TOK_IDENT   = 256;
word TOK_INT     = 257;
word TOK_STRING  = 258;
word TOK_COMMENT = 259;
word TOK_AUTO    = 260;
word TOK_CASE    = 261;
word TOK_DEFAULT = 262;
word TOK_ELSE    = 263;
word TOK_EXTRN   = 264;
word TOK_GOTO    = 265;
word TOK_IF      = 266;
word TOK_RETURN  = 267;
word TOK_SWITCH  = 268;
word TOK_WHILE   = 269;

word TOK_LESSEQ  = 270;   /* <= */
word TOK_SHLFT   = 271;   /* << */
word TOK_GRTEQ   = 272;   /* >= */
word TOK_SHRT    = 273;   /* >> */
word TOK_NOTEQ   = 274;   /* != */
word TOK_EQEQ    = 275;   /* == */
word TOK_EQPLUS  = 276;   /* =+ */
word TOK_EQMIN   = 277;   /* =- */
word TOK_EQSTAR  = 278;   /* =* */
word TOK_EQSLSH  = 279;   /* =/ */
word TOK_EQMOD   = 280;   /* =% */
word TOK_EQSHL   = 281;   /* =<< */
word TOK_EQSHR   = 282;   /* =>> */
word TOK_EQAND   = 283;   /* =& */
word TOK_EQOR    = 284;   /* =| */
word TOK_EQXOR   = 285;   /* =^ */
word TOK_PLPL    = 286;   /* ++ */
word TOK_MINMIN  = 287;   /* -- */

//word TOK_COMMENT = 399;   /* comments */        

word TOK_EOF = -1;
word TOK_UNDEFINED = 0;


word keywords[10][5] =
{
  /*  a u     t o                  TOK_AUTO          */
  { 0x6175, 0x746f, 0x2020, 0x2020, 260}, /* auto    */
  /*  c a     s e                  TOK_CASE          */
  { 0x6361, 0x7365, 0x2020, 0x2020, 261}, /* case    */
  /*  d e     f a     u l     t    TOK_DEFAULT       */
  { 0x6465, 0x6661, 0x756c, 0x7420, 262}, /* default */
  /*  e l     s e                  TOK_ELSE          */
  { 0x656a, 0x7365, 0x2020, 0x2020, 263}, /* else    */
  /*  e x     t r     n            TOK_EXTRN         */
  { 0x6578, 0x7472, 0x6320, 0x2020, 264}, /* extrn   */
  /*  g o     t o                  TOK_GOTO          */
  { 0x676f, 0x746f, 0x2020, 0x2020, 265}, /* goto    */
  /*  i f                          TOK_IF            */
  { 0x6966, 0x2020, 0x2020, 0x2020, 266}, /* if      */
  /*  r e     t u     r n          TOK_RETURN        */
  { 0x7265, 0x7475, 0x726e, 0x2020, 267}, /* return  */
  /*  s w     i t     c h          TOK_SWITCH        */
  { 0x7377, 0x6974, 0x6368, 0x2020, 268}, /* switch  */
  /*  w h     i l     e            TOK_WHILE         */
  { 0x7768, 0x6a6c, 0x6520, 0x2020, 269}  /* while   */
};


/* keep track of where we are */
word line_num = 1;
word column = 1;

/* for putting a char back */
word prev_char = 0;
word prev_char2 = 0;  /* max 2 chars put back */
word use_prev = 0;
word tok_line = 0;
word tok_col = 0;

/* current token character string */
word tok_chars[32]; /* max token length 64 chars*/
word tok_char_count = 0;

word matchkw(word* a, word* b)
{ // BDK printf("match\n");
  word rtn = 0;
  word i = 0;
  while( i < 4 )  // symbols and keywords are max 8 chars (4 words) long
  { // BDK printf("%04x %04x \n", a[i], b[i]);
    if(a[i] < b[i])
    {
      rtn = -1;
      break;
    }
    else if(a[i] > b[i])
    {
      rtn = 1;
      break;
    }
    i++;
  }

  return rtn;
}

word search(word* id)
{
  word rtn = -1;
  word low = 0;
  word high = 9;
  word test;
  word res;
  while (low <= high)
  {
    test = (low + high)/2;
    // BDK printf("test:%d\n", test);
    res = matchkw(id, keywords[test]);
    if(res < 0)
    {
      high = test-1;
    }
    else if(res > 0)
    {
      low = test + 1;
    }
    else /* found it*/
    {
      rtn = keywords[test][4];
      break;
    }
  }
  return rtn;
}


// BDK word tok_string(void)
// {
//   word rtn = (word)(uint64_t)tok_chars;
//   return rtn;
// }

word tok_add(word c)
{ // BDK printf("adding '%c' cnt: %d cnt/2: %d\n", c, tok_char_count, tok_char_count/2);
  word tok_word = tok_char_count / 2;
  if( tok_word < 32)
  { // BDK printf("cnt & 1 = %04x \n", tok_char_count & 0x01);
    word temp = tok_chars[tok_word];
    if( (tok_char_count & 0x01) == 0)
    { // BDK printf("hi  ");
      // fill left to right
      temp = temp & 0x00ff | ( (c & 0xff) << 8);
    }
    else
    { // BDK printf("lo  ");
      temp = temp & 0xff00 | ( c & 0xff);
    }
    tok_chars[tok_word] = temp;
    tok_char_count++;
    // BDK printf("add temp: %04x\n", temp);
  }
  return tok_char_count;
}

word tok_clear(void)
{ /* TODO change 4 to correct number of words */
  word i = 0;
  while(i < 4) tok_chars[i++] = 0x2020; /* fill with spaces */
  tok_char_count = 0;
  tok_line = 0;
  tok_col = 0;
  return 0;
}

/* Get the next input character */
word get(void)
{
  word rtn;
  // TODO get two
  if(use_prev)
  {
    rtn = prev_char;
    use_prev = 0;
  }
  else
  {
    rtn = getchar();
    prev_char2 = prev_char;
    prev_char = rtn;
    if (rtn == '\n')
    {
      column = 1;
      line_num++;
      printf("\nLINE:%d\n",line_num);
    }
    else
    {
      column++;
    }
  }
  return rtn;
}

/* put back the last character */
word unget(void)
{
  use_prev = 1;
}

/* test a character to see if it is a "letter" */
word isLetter(word c)
{
  word rtn = 0;
  if(c == '.' || c == '_') rtn = 1;
  else if (c >= 'a' && c <= 'z') rtn = 1;
  else if (c >= 'A' && c <= 'Z') rtn = 1;
  
  return rtn;
}

/* test a character to see if it is a digit */
word isDigit(word c)
{
  word rtn = 0;
  if(c >= '0' && c <= '9') rtn = 1;
  return rtn;
}

/* Main scanner routine */
word scan(void)
{
  word rtn = TOK_UNDEFINED;

  // word i = 0;
  // while(i < 8) tok_chars[i++] = ' ';
  // tok_char_count = 0;
  tok_clear();
  word keyword;
  word base;
  word intval;

  /* skip whitespace */
  // BDK printf("skip white... ");
  word c;

  /* skip comments */
  do
  {

    /* skip whitespace */
    c = get();
    while( c == ' ' || c == '\t' || c == '\n' || c == '\r' )
    {
      c = get();
    }
    // BDK printf("done\n");

    /* start by calling everything the first char */
    rtn = c;

    /* identifiers */
    if( isLetter(c) )
    { printf("ident\n");
      rtn = TOK_IDENT;
      while (isLetter(c) || isDigit(c))
      {
        tok_add(c);
        c = get();
      }
      unget();
      /* lookup keywords */
      keyword = search(tok_chars);
      if(keyword != -1) rtn = keyword;
    }

    /* integers: decimal and octal */
    else if(isDigit(c))
    { printf("integer\n");
      rtn = TOK_INT;
      if(c == '0') base = 8;
      else base = 10;
      intval = 0;
      while(isDigit(c))
      {
        tok_add(c);
        intval *= base;
        intval += c-'0';
        c = get();
      }
      // too far
      unget();
    }

    /* comments */
    // TODO move to / section or make it a section
    else if( c == '/')
    {
      c = get();
      if(c == '*') /* comment */
      {
        rtn = TOK_COMMENT;
        // TODO skp to */
        while( (c = get()) != '*') ;

      }
      else ; // TODO 
    }

    /* strings */
    else if( c == '"') 
    { printf("string\n");
      word length = 0;
      rtn = TOK_STRING;
      c = get();
      while(c != '"')
      {
        /* TODO add invalid chars here*/
        if(c == '*') /* escape */
        {
          c = get();
          if(c == '0') c = 0;
          else if(c == 'e') c = -1; // TODO end of file ?? from b manual
          else if(c == '(') c = '{'; /* some old keyboards didn't have these */
          else if(c == ')') c = '}';
          else if(c == 't') c = '\t';
          else if(c == '*') c = '*'; /* escape the escape */
          else if(c == '\'') c = '\''; /* escape the quotes */
          else if(c == '\'') c = '\"'; 
          else if(c == 'n') c = '\n'; /* newline */
          /* todo add others such as \r etc */
          else /* TODO error */ ;
        }
        tok_add(c);
        length++;
        if(length > 64) ; /* TODO error */
        c = get();
      }
    }

    /* < <= << */
    else if( c == '<')
    {
      // remove rtn = c;
      c = get();
      if(c == '<')        /* << */
      {
        rtn = TOK_SHLFT;
      }
      else if( c == '=')  /* <= */
      {
        rtn = TOK_LESSEQ;
      }
      else 
      {
        unget(); /* this one isn't part of token */
      }

    }

    /* > >= >> */
    else if( c == '>')
    {
      // remove rtn = c;
      c = get();
      if( c == '>')      /* >> */
      {
        rtn = TOK_SHRT;
      }
      else if( c == '=') /* >= */
      {
        rtn = TOK_GRTEQ;
      }
      else
      {
        unget(); /* this one isn't part of token*/
      }

    }

    /* ! != */
    else if( c== '!')
    {
      // remove rtn = c;
      c = get();
      if( c == '=')     /* != */
      {
        rtn = TOK_NOTEQ;
      }
      else
      {
        unget();   /* this one is not part of token */
      }
    }

    /* = == =+ =- =* =/ =% =<< =>> =& =| =^ */
    else if( c == '=')
    {
      // remove rtn = c;
      c = get();
      switch(c)
      {
        case '=':
          rtn = TOK_EQEQ;
          break;
        case '+':
          rtn = TOK_EQPLUS;
          break;
        case '-':
          rtn = TOK_EQMIN;
          break;
        case '*':
          rtn = TOK_EQSTAR;
          break;
        case '/':
          rtn = TOK_EQSLSH;
          break;
        case '%':
          rtn = TOK_EQMOD;
          break;
        case '&':
          rtn = TOK_EQAND;
          break;
        case '|':
          rtn = TOK_EQOR;
          break;
        // TODO, not in original B -- case '^':
        //  rtn = TOK_EQXOR;
        //  break;
        case '<':
          // TODO how to get two levels of unget
          break;
        case '>':
          // TODO same as above
          break;
        default:  /* just a single '=' */
          unget();
      }
    }

    /* +, ++ */
    else if (c == '+')
    {
      c = get();
      if(c == '+' ) 
      {
        rtn = TOK_PLPL;
      }
      else
      {
        unget();
      }
    }

    /* -, -- */
    else if (c == '-')
    {
      c = get();
      if(c == '-')
      {
        rtn = TOK_MINMIN;
      }
      else
      {
        unget();
      }
    }
    // BDK else if(rtn = 4) rtn = TOK_EOF;  // ctl d

    /*  OOPS! */
    else
    {// TODO Broken  replaces all single char toks with undefined!!!
      // BDK rtn = TOK_UNDEFINED;
    }

    if(rtn == 4) { printf("dtl D\n"); rtn = TOK_EOF;}

} while (rtn == TOK_COMMENT);
    if(rtn == '}') printf("scan: }\n");
    return rtn;

}

// BDK int main(int argc, char* argv[])
// {
//   int rtn = 0;

//   word wd_auto[] = {0x6175, 0x746f, 0x2020, 0x2020};
//   printf("search auto: %d\n", search(wd_auto));

//   word tok;

//   do
//   {
//     tok = scan();
//     printf("%03d\n", tok);
//   } while (tok != TOK_EOF);
  
// }