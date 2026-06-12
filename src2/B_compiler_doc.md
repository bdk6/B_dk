B Compiler Doc
wrc 20240926

B Machine
Word oriented (16 bit) with 8 bit opcodes.  Code can be on 8 bit boundaries 
but all jump destinations must be on even byte (word) boundaries.
Memory consists of 65536 bytes (32768 words).  Words are big-endian (MSB first).
Numbers are generally treated as two's complement.


OPCODES
   0 NOP    No Operation
     Does nothing
   1 PHC    PusH Constant
     Word following opcode is pushed to stack.
     
   2 FCH    FetCH
     Word located at address replaces the address on top of stack.

   3 SWP    SWap
     Top of stack and second on stack swap places, leaving top as second entry.
     
word OP_NEG = 4;
word OP_NOT = 5;
word OP_MUL = 6;
word OP_DIV = 7;
word OP_MOD = 8;