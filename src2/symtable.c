/* symbol table for B compiler
 * copyright 2024 William R Cooke */

#include "definitions.h"
#include "symtable.h"

// Name:       8 chars, 4 words, space (null? eot?) filled
// Address:    1 word  absolute or offset on stack
// Value?      fn: num parms, vect: size
// Flags:     {auto, extrn, intern, ...} fn? vector?
// Total 6/7 words

word numSyms = 100;
word symWords = 7;
word symbols[700];  // 100 symbols * 7 words each
word nameLen = 4;
word addrOff  = 4;
word valOff  = 5;
word flagOff  = 6;
word symSize = 7;

/* flag definitions */
/* [defined | vector | ... | extrn, auto, intern ] */

word sym_def = 0x8000;  /* 1 < 15 */
word sym_ext = 0x0002;  /* extrn */
word sym_int = 0x0001;  /* internal */
word sym_aut = 0x0000;  /* auto */
word sym_loc = 0x4000;  // Local symbol */


/* fn call stack frame 
   parm n      off n+4
   parm n-1    off n-1+4
   parm ...
   parm 1      off 5
   parm 0      off 4
   count       off 3
   ret val     off 2
   ret addr    off 1
   prev fp     off 0 <-- FP points here
   auto 0      off -1
   auto 1      off -2
   auto ...
   auto n-1    off n-1-1
   auto n      off n-1
*/

//word nextExtrn = 0;
// word nextLocal = 99;  // table size
// TODO change to only fill one direction
word nxtSym = 0;

/* match two symbol names */
/* param[in] s1 ptr to first name */
/* param[in] s2 ptr to second name */
/* return true if match, else 0 */
word matchSym(word* s1, word* s2)
{
  word rtn = 1;
  word count = 0;
  while(count < nameLen)
  {
    if(*s1++ != *s2++) rtn = 0;
  }

  return rtn;
}

/* match name and symbol entry */
/* param[in] s1 ptr to name */
/* param[in] entry table entry number */
/* return true if match, else 0*/
word match(word* s1, word entry)
{
  word rtn = 0;
  word* tblstring;
  word count = 0;
  tblstring = &symbols[entry * symSize];

  return (matchSym(s1, tblstring));
}

word findSym(word* name, word isLocal)
{
  word rtn = -1;
  // TODO search
  // TODO reverse and start and end
  word i = 0;

  i = nxtSym;
  while(--i >= 0)
  {
    if(match(name, i)) 
    {
      rtn = i;
      goto leave;
    }
  }
  //while(i < nextExtrn)
  //{
  //
  //}
  leave: return rtn;
}

/* Insert a symbol into table */
/* param[in] sym The symbol to insert */
/* param[in] isLocal 1 if this is local */
/* return index or -1 if already present*/
word insertSym(word * sym, word isLocal)
{
  word rtn = -1;
  word index;
  word count = 0;
  index = findSym(sym, isLocal);
  if(index >= 0)  // was in table
  {
    if(isLocal && symgetlocal(index))
    {
      // TODO error
      return rtn;
    }
    else if(!isLocal && !symgetlocal(index))
    {
      // TODO error
      return rtn;
    }

  }
  // if(isLocal) 
  // {
  //   rtn = nextLocal;
  //   index = symWords * nextLocal++;
  //   // TODO search
  // }
  // else
  // {
  //   rtn = nextExtrn;
  //   index = symWords * nextExtrn++;
  //   // TODO search
  // }
  index = nxtSym;
  while(count < symWords)
  {
    symbols[index++] = sym[count++];
  }
  

  return rtn;
}

word symsetadr(word entry, word addr)
{
  word rtn = 0;
  word index = entry * symWords + 4;  // addr is offset 4
  symbols[index] = addr;
  rtn = addr; // TODO what shall we return

  return rtn;
}

word symgetadr(word entry)
{
  word rtn = 0;
  word index = entry * symWords + 4;
  rtn = symbols[index];

  return rtn;
}

word symsetval(word entry, word val)
{
  word rtn = 0;
  word index = entry * symWords + 5;
  symbols[index] = val;

  return rtn;
}

word symgetval(word entry)
{
  word rtn = 0;
  word index = entry * symWords +5;
  rtn = symbols[index];

  return rtn;
}

// word sym_def = 0x8000;  /* 1 < 15 */
// word sym_ext = 0x0002;  /* extrn */
// word sym_int = 0x0001;  /* internal */
// word sym aut = 0x0000;  /* auto */

word symsetclass(word entry, word cls)
{
  word rtn = 0;
  word index = entry * symWords + 6;
  rtn = symbols[index];
  rtn &= 0xfffc | (cls & 0x0003);
  symbols[index] = rtn;
  return rtn;
}

word symgetclass(word entry)
{
  word rtn = 0;
  word index = entry * symWords + 6;
  rtn = symbols[index];
  rtn &= 0x0003;
  return rtn;
}

// defined = 0x8000
// any true value sets the flag
word symsetdefined(word entry, word def)
{
  word rtn = 0;
  word index = entry * symWords + 6;
  rtn = symbols[index];
  if(def) rtn |= sym_def;
  else rtn &= ~sym_def;

  return rtn;
}

word symgetdefined(word entry)
{
  word rtn = 0;
  word index = entry * symWords + 6;
  rtn = symbols[index] & sym_def;
  if(rtn) rtn = 1;
  else rtn = 0;

  return rtn;
}

/* return true if this entry is a local */
word symgetlocal(word entry)
{
  word rtn = 0;
  word index = entry * symWords + 6;
  rtn = symbols[index] & sym_loc;
  if(rtn) rtn = 1;

  return rtn;
}

/* Remove all locals */
/* return number locals removed */
word removeLocals(void)
{
  word rtn = 0;
  // TODO write this
  word i;
  i = nxtSym;
  while(--i >= 0)
  {
    if(symgetlocal(i))
    {
      nxtSym--; /* delete this entry */
      rtn++;
    }
    else goto leave;
  }

  leave: return rtn;
}
