#include <string.h>
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
 * Flags
 * TODO
 * auto extern fn label ... initialized
 ********************************************************************** */

/* ***********************************************************************
 * Value
 * What goes into the value field varies:
 * Simple extern variable:          The address in DATA/BSS section
 * extern vector:                   The addr. of ptr to vec in DATA/BSS
 * function:                        The address of function in CODE
 * fn parameter:                    Offset in param area (from param ptr)
 * auto variable:                   Offset in local area (from frame ptr)
 * 
 * If variables are initialized (extrn only) the initial values will be 
 * placed into the DATA section and written out as records separate from 
 * the code.
 * 
 * Sections:
 *   DATA:        Holds all initialized static data (extrn).
 *   BSS:         Holds all uninitialized static data (extrn).
 *   CODE:        Holds all generated machine instructions.
 *   STACK:       Holds all params, autos, return addr, temporaries
 *   HEAP:        Holds whatever the programmer wants to put there.
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
// TODO word stringtable[16000];
char stringtable[16000];

/* ***********************************************************************
 * stringpointer
 * @brief Index of next free spot in stringtable
 ********************************************************************** */
word stringpointer;

/* ***********************************************************************
 * @fn sym_initialize
 * @brief Clears the symbol table before use.
 * @return 0
 ********************************************************************** */
word sym_initialize()
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
  stringpointer = 1;  /* Can't be 0: 0 indicates empty in symtable */

  return(0);
}

/* ***********************************************************************
 * @fn comparesym
 * @brief compares an input string with table entry
 * @param[in] name Input string to compare.
 * @param[in] index Index of table entry to compare.
 * @return 0 if match, 1 if less, 2 if greater.
 ********************************************************************** */
//word comparesym(word* name, word index)
word comparesym(char* name, word index)
{
  // This is the C version.  Part of B version commented out below.
  word match;
  int compare;
  if(stringtable[index] == 0);
  compare = strcmp(name, &stringtable[index]);
  if(compare < 0)
  {
    match = 1;
  }
  else if(compare > 0)
  {
    match = 2;
  }
  else
  {
    match = 0;
  }
  
  //word rtn = 2; /* start by assuming we found it */
  //word address = symtable[(index - 1) * 3]; /* 3 is # words per entry */
  //if(address == 0)
  //{
  //  rtn = 0;
  //}
  /* compare the strings here */
  /* kludge for using C strcmp with B-style program */
  //else
  //{
  //  int match = strcmp(
  
  //else if( /*TODO name < table */)
  //{
  //  rtn = 1;
  //}
  //else if( /* TODO name > table */)
  //{
  //  rtn = 2;
  //}
  
  return match;
}



/* ***********************************************************************
 * @fn sym_find
 * @brief find an entry in the symtable or where it would go if not there.
 * @param[in] name Pointer to the string for the symbol.
 * @return Index of the found entry or where it would go, 0 if no room.
 ********************************************************************** */
word sym_find(char* name)
{  
   word index = 1;
   word result;
   word location;
   word stringIndex;

   // if index empty return
   // compare name to index
   // if equal, return
   // if less index = index * 2
   // if greater index = index * 2 + 1
   // if table is full set index to -1 and return
   do
   {
     location = (index - 1) * 3; /* string index is first word */
     stringIndex = symtable[location];
     if(stringIndex == 0)
     {
       /* Empty -- exit */
       break;
     }
     else
     {
       result = comparesym(name, stringIndex);
       if(result == 1)  /* name < table */
       {
         index = index * 2;
       }
       else if( result == 2) /* name > table */
       {
         index = index * 2 + 1;
       }
       else
       {
         /* match */
         break;
       }
     }
   }while(index < ENTRIES);
   if(index >= ENTRIES)
   {
     index = 0;
   }

   return index;
}

/* ***********************************************************************
 * @fn sym_isThere
 * @brief Check if there is a symbol in the given index.
 * @param[in] index The table entry to check.
 * @return Zero if entry is empty, 1 if it is occupied.
 ********************************************************************** */
word sym_isThere(word index)
{
  word rtn = 0;
  word location;
  location = (index - 1) * 3;  /* check the string pointer */
  if(location != 0)            /* If it's null, no symbol */
  {
    rtn = 1;
  }
  return rtn;
}

/* ***********************************************************************
 * @fn sym_insert
 * @brief Adds a symbol to the table if not there and there is room.
 * @param[in] name Pointer to the name string.
 * @param[in] flags The "type" flags for the symbol.
 * @param[in] val The "value" of the symbol if available.
 * @return Index it went into, 0 if already there, -1 if no room in table.
 ********************************************************************** */
word sym_insert(char* name, word flags, word val)
{
  word rtn = 0;
  word index = 0;
  word location = 0;
  word length;
  
  index = sym_find(name);
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
    length = strlen(name) + 1; /* Add NULL terminator */
    //printf("name is *%s* len: %d at: %d\n", name, strlen(name) + 1, stringpointer);
    if(stringpointer + length < 16000)   /* Make sure there is room at the inn */
    {
      symtable[location] = stringpointer;  /* Save string in stringtable */
      strcpy(&stringtable[stringpointer], name);
      stringpointer += length;
      symtable[location + 1] = flags;
      symtable[location + 2] = val;
      rtn = index;
    }
    else
    {
      rtn = -1;
    }
  }
  
  return rtn;
}


/* ***********************************************************************
 * @fn sym_setFlags
 * @brief Updates the flags for a symbol already in table.
 * @param[in] index The index of table entry to update.
 * @param[in] flags The flag word to put in the table.
 * @return The old value of the flags before update.
 ********************************************************************** */
word sym_setFlags(word index, word flags)
{
  word rtn;
  word location;
  location = (index - 1) * 3 + 1; /* 3 words/entry, flags are offset 1 */
  rtn = symtable[location];
  symtable[location] = flags;
  return rtn;
}
/* ***********************************************************************
 * @fn sym_setValue
 * @brief Updates the flags for a symbol already in table.
 * @param[in] index The index of table entry to update.
 * @param[in] flags The flag word to put in the table.
 * @return The old value of the flags before update.
 ********************************************************************** */
word sym_setValue(word index, word val)
{
  word rtn;
  word location;
  location = (index - 1) * 3 + 2; /* 3 words/entry, val is offset 2 */
  rtn = symtable[location];
  symtable[location] = val;
  return rtn;
}

/* ***********************************************************************
 * @fn sym_getFlags
 * @brief Retrieves the stored flags of a symbol in the table.
 * @param[in] index The index of table entry to get.
 * @return The flags stored in the table. Return 0 if invalid.
 ********************************************************************** */
word sym_getFlags(word index)
{
  word rtn = 0;
  word location;
  location = (index - 1) * 3 + 1; /* 3 words/entry, flags are offset 1 */
  rtn = symtable[location];
  return rtn;
}

/* ***********************************************************************
 * @fn sym_getValue
 * @brief Retrieves the stored value of a symbol in the table.
 * @param[in] index The index of table entry to get.
 * @return The value stored in the table. Return 0 if invalid.
 ********************************************************************** */
word sym_getValue(word index)
{
  word rtn = 0;
  word location;
  location = (index - 1) * 3 + 2; /* 3 words/entry, value is offset 2 */
  rtn = symtable[location];
  return rtn;
}


/* ***********************************************************************
 * @fn sym_dumpTable
 * @brief Prints out all used entries in sym table
 * @return Number of symbols printed
 ********************************************************************** */
word sym_dumpTable(void)
{
  word rtn = 0;
  word index = 1;
  word location;

  printf("idx flags value    at    string \n");
  while(index <= ENTRIES)
  {
    location = (index - 1) * 3;
    
    if(symtable[location] != 0)
    {
      printf("%3d: %04x : %04x : %04x : %s \n",
             //&stringtable[symtable[location]],
             index,
             symtable[location + 1],
             symtable[location + 2],
             symtable[location],
             &stringtable[symtable[location]]
        );
      rtn++;
    }
    index++;
  }
  return rtn;
}



