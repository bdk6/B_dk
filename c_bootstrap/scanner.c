#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <assert.h>
#include "scanner.h"
#include "symtable.h"


static word linenumber = 0; /* Current line number of input file */
//static word ident[32];  /* Holding area for token characters (e.g. ident)*/
static char ident[32];   // having trouble with words
static word num;        /* Holds value for scanned constants */

FILE* infile = NULL;

word savedChar[2]; /* putback buffer for input routine */
word savedChars;   /* count of characters in save buffer */


/* ******    Temporary stuff until combine all files ***************/
word stringtable[16000];   /* Place to hold ident strings */
word stringpointer = 0;    /* Where next char goes in stringtable */
/* The two above are/will be defined in symtable.c / symtable.h */

/* for debugging */
char* tokNames[] =
{
  "TOK_INT",
  "TOK_IDENT",
  "TOK_STRING",
  "TOK_AUTO",
  "TOK_EXTRN",
  "TOK_CASE",
  "TOK_IF",
  "TOK_ELSE",
  "TOK_WHILE",
  "TOK_SWITCH",
  "TOK_GOTO",
  "TOK_RETURN"
};
  

/* ***********************************************************************
 * @fn scanfile
 * @brief Opens a file for scanning
 * @param[in] filename Name of the file to open
 * @return 0 on success, -1 on error
 ********************************************************************** */
word scanfile(char* filename)
{
  word rtn = 0;
  assert(filename != NULL);
  infile = fopen(filename, "r");
  if(infile == NULL)
  {
    rtn = -1;
  }
  savedChars = 0;    /* how many characters have we "ungotten?" */
  linenumber = 1;
  return rtn;
}

/* ***********************************************************************
 * @fn next
 * @brief Gets next character from input.
 * @return The next character or EOF
 ********************************************************************** */
word next()
{
  word rtn;
  rtn = 0;
  if(savedChars > 0)
  {
    savedChars--;
    rtn = savedChar[savedChars];
  }
  else
  {
    rtn = getc(infile);
  }
 
  return rtn;
}

/* ***********************************************************************
 * @fn unnext
 * @brief Puts a character (up to 2) back in input stream.
 ********************************************************************** */
word unnext(word ch)
{
  if(savedChars < 2)
  {
    savedChar[savedChars] = ch;
    savedChars++;
  }
  return 0;
}

/* ***********************************************************************
 * @fn iskeyword
 * @brief Determines if identifier is a keyword.
 * @param[in] ident Pointer to the identifier to test.
 * @return Token for the keyword or TOK_IDENT if not a keyword.
 ********************************************************************** */
word iskeyword(char* ident) //word ident)
{
  word rtn = TOK_IDENT;

  /* TODO: fill this in */
  if(strcmp(ident,"auto") == 0)
    rtn = TOK_AUTO;
  else if(strcmp(ident,"extrn") == 0)
    rtn = TOK_EXTRN;
  else if(strcmp(ident,"if") == 0)
    rtn = TOK_IF;
  else if(strcmp(ident,"else") == 0)
    rtn = TOK_ELSE;
  else if(strcmp(ident, "while") == 0)
    rtn = TOK_WHILE;
  else if(strcmp(ident, "return") == 0)
    rtn = TOK_RETURN;
  else if(strcmp(ident, "goto") == 0)
    rtn = TOK_GOTO;
  else if(strcmp(ident, "switch") == 0)
    rtn = TOK_SWITCH;
  else if(strcmp(ident, "case") == 0)
    rtn = TOK_CASE;
  
  return rtn;
}

/* ***********************************************************************
 * @fn convertEscape
 * @brief converts an escape sequence to a char in strings and constants.
 * @param[in] ch Escaped character to convert
 * @return The converted character
 * ******************************************************************** */
word convertEscape(word ch)
{
  switch(ch)
  {
  case '0':         /*  NULL */
    ch = 0;
    break;
  case 'e':         /* end of file/string */
    ch = 0;         /* Or EOT/0x03 in original B */
    break;
  case '(':         /* sub for { */
    ch = '{';
    break;
  case ')':         /* sub for } */
    ch = '}';
    break;
  case 't':         /* tab */
    ch = 9;
    break;
  case '*':         /* * */
    // leave it
    break;
  case '\'':        /* '  single quote */
    /* hmmm */
    break;
  case '"':         /*  " double quote */
    /* leave it */
    break;
  case 'n':         /* newline (linefeed 10) */
    ch = 10;
    break;
  case 'r':         /* carriage return  13 */
    ch = 13;
    break;
  case 'a':         /* alert/bell 7 */
    ch = 7;
    break;
  default:          /* hmmm, just leave it I guess */
    break;
  } /* switch */


  return ch;
}


/* ***********************************************************************
 * @fn scan
 * @brief Gets next token from input.
 * @return The type of next token in input stream or TOK_EOF.
 ********************************************************************** */
word scan(void)
{
  word tok = TOK_UNDEFINED;
  word ch, ch1, ch2;
  word stringcount;
  ident[0] = 0;
  num = 0;
  stringcount = 0;

  /* skip whitespace */
  do
  {
    ch = getc(infile);
    /*  ... */
    if(ch == '\n')
    {
      linenumber++;
      printf("\n; Line: %d\n", linenumber);
    }
    
  }while(ch == ' ' || ch == '\t' || ch == '\n');

  /* Assume single character token and use it as token type */
  tok = ch;

  /* Now check and modify our assumption if needed */
  
  /* ******************** Identifiers ******************** */
  if(ch == '_' || ch >= 'a' && ch <= 'z' || ch >= 'A' && ch <= 'Z')
  {
    tok = TOK_IDENT;
    stringcount = 0;
    while(ch == '_' || ch >= 'a' && ch <= 'z'
          || ch >= 'A' && ch <= 'Z' || ch >= '0' && ch <= '9')
    {
      if(stringcount < 31)
      {
        ident[stringcount++] = ch;
      }
      ch = getc(infile);
    }
    ident[stringcount] = 0;   /* terminate the string */
    /* this character is NOT part of identifier */
    ungetc(ch, infile);
    tok = iskeyword(ident);
//    printf("IDENT characters are: *%s*\n", ident);
    //printf("%s %d %s \n", tokNames[tok-256],stringcount,ident);
    //for(int i = 0; i < stringcount; i++) putchar(ident[i]);
    //putchar('\n');
  }

  /* ******************** Numbers ******************** */
  else if(ch >= '0' && ch <= '9')
  {
    word base;
    base = 10;
    if(ch == '0')
    {
      base = 8;
    }
    
    tok = TOK_INT;
    stringcount = 0;
    while(ch >= '0' && ch <= '9')
    {
      if(stringcount < 31)
      {
        ident[stringcount++] = ch;
      }
      num = num * base + (ch - '0');
      ch = getc(infile);
    }
    ident[stringcount] = 0;
    /* this character is NOT part of number */
    ungetc(ch, infile);
  }
  
  /* Character constant */
  /* escapes 
   *    *0   null
   *    *e   end of file
   *    *(   {
   *    *)   }
   *    *t   tab
   *    **   *
   *    *'   '
   *    *"   "
   *    *n   new line
   * --------------------
   *  added by me
   *    *r   return
   */
  /* ******************** Character Constants ******************** */
  else if(ch=='\'')
  {
    word ccon;   /* holds the value */   /* TODO num? */
    ccon = 0;
    tok = TOK_INT;
    ch = getc(infile);
    while((ch = getc(infile)) != '\'')
    {
      if(ch == '*')  /* escape */
      {
        ch = getc(infile);
        ch = convertEscape(ch);
      } /* if (escapes) */
      ccon <<= 8;
      ccon += ch;
      num = ccon;
    } /* while */
  }

  /* ******************** Strings ********************* */
  else if(ch == '"')
  {
    tok = TOK_STRING;
    ch = getc(infile);
    num = 9999;      /* TODO set to data counter address */
    while((ch = getc(infile)) != '"')
    {
      if(ch == '*')   /* escape */
      {
        ch = getc(infile);
        ch = convertEscape(ch);
      }
    }
  }
  /* TODO finish this */
    
      
    

  /* ******************** + tokens ******************** */
  else if(ch == '+')
  {
    ch1 = getc(infile);
    if(ch1 == '+')
    {
      tok = TOK_INC;
    }
    else
    {
      ungetc(ch1, infile);
    }
  }
  /* ******************** - tokens ******************** */
  else if(ch == '-')
  {
    ch1 = getc(infile);
    if(ch1 == '-')
    {
      tok = TOK_DEC;
    }
    else
    {
      ungetc(ch1, infile);
    }
  }
  /* ******************** ! tokens ******************* */
  else if(ch == '!')
  {
    ch1 = getc(infile);
    if(ch1 == '=')
    {
      tok = TOK_NOT_EQ;
    }
    else
    {
      ungetc(ch1, infile);
    }
  }
  /* ******************** < tokens ******************** */
  else if(ch == '<')
  {
    ch1 = getc(infile);
    if(ch1 == '=')
    {
      tok = TOK_LESS_EQ;
    }
    else if(ch1 == '<')
    {
      tok = TOK_SHIFT_LEFT;             /* << */
    }
    else
    {
      ungetc(ch1, infile);
    }
  }
  /* ******************** > tokens ******************** */
  else if(ch == '>')
  {
    /* TODO ... '>=', '>>' */
    ch1 = getc(infile);
    if(ch1 == '=')
    {
      tok = TOK_GREAT_EQ;
    }
    else if(ch1 == '>')                 /* >> */
    {
      tok = TOK_SHIFT_RIGHT;
    }
    else
    {
      ungetc(ch1, infile);
    }
  }
  /* ******************** = tokens ******************** */
  else if(ch == '=')
  {
    /* TODO ... '==', '=??' (?? = binary) */
    /* ==, =|, =&, =<, =>, =-, =+, =%, =*, =/ */
    /* ===, =!=, =>=, =<=, =<<, =>>, */
    ch1 = getc(infile);

    /* =| */
    if(ch1 == '|')                      /* =| */
    {
      tok = TOK_EQ_OR;                  /* =| */
    }
    /* =& */
    else if(ch1 == '&')
    {
      tok = TOK_EQ_AND;                 /* =& */
    }
   
    /* ==, === */
    else if(ch1 == '=')
    {
      tok = TOK_EQ_EQ;                  /* == */
      ch2 = getc(infile);
      if(ch2 == '=')
      {
        tok = TOK_EQ_EQ_EQ;             /* === */
      }
      else
      {
        ungetc(ch2, infile);
      }
    }
    else if(ch1 == '!')
    {
      /* TODO!   there is no =! only =!= need buffer */
    }
    else if(ch1 == '<')
    {
      tok = TOK_EQ_LESS;                /* =< */
      ch2 = getc(infile);
      if(ch2 == '=')
      {
        tok = TOK_EQ_LESS_EQ;           /* =<= */
      }
      else if(ch2 == '<')
      {
        tok = TOK_EQ_SHIFT_LEFT;
      }
      else
      {
        ungetc(ch2, infile);
      }
    }
    else if(ch1 == '>')
    {
      tok = TOK_EQ_GREAT;               /* => */
      ch2 == getc(infile);
      if(ch2 == '=')
      {
        tok = TOK_EQ_GREAT_EQ;          /* =>= */
      }
      else if(ch2 == '>')
      {
        tok = TOK_EQ_SHIFT_RIGHT;       /* =>> */
      }
      else
      {
        ungetc(ch2, infile);
      }
    }
    else if(ch1 == '-')
    {
      tok = TOK_EQ_MINUS;              /* =- */
    }
    else if(ch1 == '+')
    {
      tok = TOK_EQ_PLUS;               /* =+ */
    }
    else if(ch1 == '%')
    {
      tok = TOK_EQ_MOD;                /* =% */
    }
    else if(ch1 == '*')
    {
      tok = TOK_EQ_STAR;                /* =* */
    }
    else if(ch1 == '/')
    {
      tok = TOK_EQ_SLASH;               /* =/ */
    }
    else
    {
      ungetc(ch1, infile);
    }
  }

  if(tok == 65535)                /* kludge! */
  {
    tok = TOK_EOF;
  }
  
//  printf("TOK: %3d\n", tok);
  return tok;
}

/* ***********************************************************************
 * @fn main
 * @brief Temporary main function to "test" scanner
 * ******************************************************************** */
int scanmain(int argc, char** argv)
{
  int rtn = 0;
//  printf("argc = %d \n", argc);
  if(argc >1)
  {
    for(int arg = 0; arg < argc; arg++)
    {
//      printf("arg %2d: %s \n", arg, argv[arg]);
    }
    for(int arg = 1; arg < argc; arg++)
    {
      scanfile(argv[arg]);
      word tok;
      do
      {
        tok = scan();
//        printf("tok = %3d \n", tok);
        if(tok == TOK_IDENT)
        {
          int i = 0;
          while(ident[i] != 0)
          {
            putchar(ident[i++]);
          }
          putchar('\n');
        }
      }while(tok > 0);
    }
    
  }
//  printf("stringpointer is %d \n", stringpointer);
  


  return rtn;
}


  
/* *************************************************************************
 * @fn getNumber
 * @brief Get the most recent scanned number value.
 * @return Most recent scanned number.
 * ********************************************************************** */
word getNumber(void)
{
  return num;
}
  
  
/* *************************************************************************
 * @fn getText
 * @brief Get a pointer to the token text.
 * @return Pointer to current token text string.
 * ********************************************************************** */
char* getText(void)
{
//  printf("getText returning:%s:\n", ident);
  return ident;
}

/* *************************************************************************
 * @fn getLineNumber
 * @brief Get the current source file line number.
 * @return Current line number.
 * ********************************************************************** */
word getLineNumber(void)
{
  return linenumber;
}
