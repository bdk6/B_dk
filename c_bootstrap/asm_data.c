/* ***********************************************************************
 * @file asmdata.c
 * @copy Copyright 2022 William R Cooke
 * @brief Data needed by the VM assembler
 * ******************************************************************** */
#include "definitions.h"
#include "asm_data.h"

char* opcodeStrings[] =
{
  "NOP",         // 0
  "ADD",         // 1
  "SUB",         // 2
  "MUL",         // 3
  "DIV",         // 4
  "MOD",         // 5
  "AND",         // 6
  "OR",          // 7
  "XOR",         // 8

  "CPL",         // 16
  "NEG",         // 17
  "NOT",         // 18

  "BR",          // 32
  "BSR",         // 33
  "BRZ",         // 34
  "BRNZ",        // 35
  "RET",         // 36
  "CALL",        // 37

  "FETCH",       // 48
  "STORE",       // 49
  "PSHI",        // 50
  "POP",         // 51
  "DUP",         // 52
  "DROP",        // 53

  "SETSP",       // 64
  "GETSP",       // 65
  "SETPP",       // 66
  "GETPP",       // 67
  "SETFP",       // 68
  "GETFP",       // 69
  "SPI",         // 70
  "PPI",         // 71
  "FPI",         // 72

  "OUTCH",       // 250
  "INCH",        // 251

  "TON",         // 252
  "TOFF",        // 253

  "HALT"         // 255
};

word opcodeValues[] =
{
  /* code   name           code   words */
  0,    // "NOP",         // 0      1
  1,    // "ADD",         // 1      1
  2,    // "SUB",         // 2      1
  3,    // "MUL",         // 3      1
  4,    // "DIV",         // 4      1
  5,    // "MOD",         // 5      1
  6,    // "AND",         // 6      1
  7,    // "OR",          // 7      1
  8,    // "XOR",         // 8      1

  16,   // "CPL",         // 16     1
  17,   // "NEG",         // 17     1
  18,   // "NOT",         // 18     1

  32,   // "BR",          // 32     2
  33,   // "BSR",         // 33     2
  34,   // "BRZ",         // 34     2
  35,   // "BRNZ",        // 35     2
  36,   // "RET",         // 36     1/2 (pop parameters?)
  37,   // "CALL",        // 37     2

  48,   // "FETCH",       // 48     1
  49,   // "STORE",       // 49     1
  50,   // "PSHI",        // 50     2
  51,   // "POP",         // 51     1
  52,   // "DUP",         // 52     1
  53,   // "DROP",        // 53     1

  64,   // "SETSP",       // 64     ?
  65,   // "GETSP",       // 65     ?
  66,   // "SETPP",       // 66     ?
  67,   // "GETPP",       // 67     1     PP -> TOS
  68,   // "SETFP",       // 68     ?
  69,   // "GETFP",       // 69     1     FP -> TOS
  70,   // "SPI",         // 70     2     IMM -> SP
  71,   // "PPI",         // 71     2     IMM -> PP
  72,   // "FPI",         // 72     2     IMM -> FP

  250,  // "OUTCH",       // 250    1
  251,  // "INCH",        // 251    1

  252,  // "TON",         // 252    1
  253,  // "TOFF",        // 253    1

  255,  // "HALT"         // 255    1
};

word opcodeSizes[] =
{
  /* code   name           code   words */
  1,    // "NOP",         // 0      1
  1,    // "ADD",         // 1      1
  1,    // "SUB",         // 2      1
  1,    // "MUL",         // 3      1
  1,    // "DIV",         // 4      1
  1,    // "MOD",         // 5      1
  1,    // "AND",         // 6      1
  1,    // "OR",          // 7      1
  1,    // "XOR",         // 8      1

  1,    // "CPL",         // 16     1
  1,    // "NEG",         // 17     1
  1,    // "NOT",         // 18     1

  2,    // "BR",          // 32     2
  2,    // "BSR",         // 33     2
  2,    // "BRZ",         // 34     2
  2,    // "BRNZ",        // 35     2
  2,    // "RET",         // 36     1/2 (pop parameters?)
  2,    // "CALL",        // 37     2

  1,    // "FETCH",       // 48     1
  1,    // "STORE",       // 49     1
  2,    // "PSHI",        // 50     2
  1,    // "POP",         // 51     1
  1,    // "DUP",         // 52     1
  1,    // "DROP",        // 53     1

  0,    // "SETSP",       // 64     ?
  0,    // "GETSP",       // 65     ?
  0,    // "SETPP",       // 66     ?
  1,    // "GETPP",       // 67     1     PP -> TOS
  0,    // "SETFP",       // 68     ?
  1,    // "GETFP",       // 69     1     FP -> TOS
  2,    // "SPI",         // 70     2
  2,    // "PPI",         // 71     2
  2,    // "FPI",         // 72     2

  1,    // "OUTCH",       // 250    1
  1,    // "INCH",        // 251    1

  1,    // "TON",         // 252    1
  1,    // "TOFF",        // 253    1

  1,    // "HALT"         // 255    1
};
