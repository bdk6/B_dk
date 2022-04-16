#include "symtable.h"

/* ***********************************************************************
 * Symbol table handling for B compiler
 * 
 * The symbol table is held as a binary tree.
 * The entries in the main part of the table are three words.
 * The first word is a pointer to the string in the string space.
 * The second word is a set of flags to indicate what the entry is (e.g)
 * function, local, extrn, etc.  The third entry is the "value" whether 
 * that be an initial value, an address, a stack address, or whatever.
 * The string space is separate to allow for variable length (i.e. long) 
 * names without wasting space.  The number of entries should be a 
 * power of two - 1 (e.g. 511). Entries is defined in the header.
 * The indexes of the table run from 1 to ENTRIES.  Index 1 is in words 
 * 0,1,2.  The left pointer of index[n] is at index[2n] and the right 
 * pointer is at index[2n+1].  A string pointer of NULL (0) means no 
 * entry at that index.
 * 
 * TODO: how to handle local symbols?
 ********************************************************************** */

/* ***********************************************************************
 * symtable
 * @brief Holds fixed part of binary tree symbol table.
 ********************************************************************** */
word symtable[ENTRIES*3];

/* ***********************************************************************
 * stringtable
 * @brief Holds variable length strings for symbol table.
 ********************************************************************** */
word stringtable[16000];

/* ***********************************************************************
 * stringpointer
 * @brief Index of next free spot in stringtable
 ********************************************************************** */
word stringpointer;

/* ***********************************************************************
 * @fn initializeTable
 * @brief Clears the symbol table before use.
 * @return 0
 ********************************************************************** */
word initializeTable()
{
  word i;
  word max;
  max = ENTRIES * 3;
  
  for(i = 0; i < max; i++)
  {
    symtable[i] = 0;
  }
  for(i = 0; i < 16000; i++)
  {
    stringtable[i] = 0;
  }
  stringpointer = 0;

  return(0);
}

/* ***********************************************************************
 * @fn comparesym
 * @brief compares an input string with table entry
 * @param[in] name Input string to compare.
 * @param[in] index Index of table entry to compare.
 * @return 0 if the table entry is empty, 1 if name<table, 2 if name>entry.
 ********************************************************************** */
word comparesym(word* name, word index)
{
  word rtn = 2; /* start by assuming we found it */
  word address = symtable[(index - 1) * 3]; /* 3 is # words per entry */
  if(address == 0)
  {
    rtn = 0;
  }
  /* compare the strings here */
  
  else if( /*TODO name < table */)
  {
    rtn = 1;
  }
  else if( /* TODO name > table */)
  {
    rtn = 2;
  }
  

  return rtn;
}



/* ***********************************************************************
 * @fn findsym
 * @brief find an entry in the symtable or where it would go if not there.
 * @param[in] name Pointer to the string for the symbol.
 * @return Index of the found entry or where it would go, -1 if no room.
 ********************************************************************** */
word findsym(word* name)
{  
   word index = 1;
   word result;

   // if index empty return
   // compare name to index
   // if equal, return
   // if less index = index * 2
   // if greater index = index * 2 + 1
   // if table is full set index to -1 and return
   do
   {
     result = comparesym(name, index);
     if(result < 0)
     {
       index = index * 2;
     }
     else if( result > 0)
     {
       index = index * 2 + 1;
     }
     else
     {
       /* match */
       break;
     }
   }while(index < ENTRIES);
   if(index >= ENTRIES)
   {
     index = -1;
   }

   return index;
}

/* ***********************************************************************
 * @fn insertsym
 * @brief Adds a symbol to the table if not there and there is room.
 * @param[in] name Pointer to the name string.
 * @param[in] flags The "type" flags for the symbol.
 * @param[in] val The "value" of the symbol if available.
 * @return Index it went into, 0 if already there, -1 if no room in table.
 ********************************************************************** */
word insertsym(word flags, word val, word* name)
{
  word rtn = 0;
  word index = 0;
  word location = 0;
  index = findsym(name);
  location = (index - 1) * 3;
  if(index < 0)  /* Not in table and no room to put it there */
  {
    rtn = -1;
  }
  else if(symtable[location] != 0) /* It's already there */
  {
    rtn = 0;
  }
  else
  {
    /* TODO put it in the table */
    /* TODO put the string in proper place */
    symtable[location] = name;
    symtable[location + 1] = flags;
    symtable[location + 2] = val;
    rtn = index;
  }
  
  return rtn;
}


word getsymflags(word index)
{
  return symtable[(index - 1) * 3 + 1];
}

word getsymvalue(word index)
{
  return symtable[(index - 1) * 3 + 2];
}

/* ***********************************************************************
 * @fn setsymflags
 * @brief Updates the flags for a symbol already in table.
 * @param[in] index The index of table entry to update.
 * @param[in] flags The flag word to put in the table.
 * @return The old value of the flags before update.
 ********************************************************************** */
word setsymflags(word index, word flags)
{
  word rtn;
  word location;
  location = (index - 1) * 3 + 1;
  rtn = symtable[location];
  symtable[location] = flags;
  return rtn;
}

word setsymvalue(word index, word val)
{
  word rtn;
  word location;
  location = (index - 1) * 3 + 2;
  rtn = symtable[location];
  symtable[location] = val;
  return rtn;
}



