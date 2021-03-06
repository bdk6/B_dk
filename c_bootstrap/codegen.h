/* ***********************************************************************
 * @file codegen.h
 * @copy Copyright 2022 William R Cooke
 * @brief Handles code generation for virtual B machine.
 * ******************************************************************** */

#define OBJ_DATA        0x11
#define OBJ_BSS         0x12
#define OBJ_CODE        0x10

word gen_initialize(char* filename);
word gen_finish();

word gen_startDataBlock(char* name);
word gen_addDataWord(word d, word reloc);
word gen_endDataBlock();

word gen_startBSSBlock();
word gen_addBSSWord(word n);
word gen_endBSSBlock();

word gen_startCodeBlock();
word gen_addCodeWord(word c, word reloc);
word gen_endCodeBlock();

word gen_if_start();
word gen_if_else(word start_addr);
word gen_if_end(word previous_addr);

word gen_cond_start();
word gen_cond_true();
word gen_cond_false(word addr1);
word gen_cond_end();

word gen_while_start();
word gen_while_exp();
word gen_while_end(word start_addr, word exp_addr);

word gen_function_params(word addr);
word gen_function_call(word addr);
word gen_function_return();
word gen_goto(word addr);

word gen_get_parameter(word offset);
word gen_get_auto(word offset);
word gen_get_extrn(word addr);

word gen_binop(word op);
word gen_unary(word op);

word gen_shl();
word gen_shr();

word gen_literal(word lit);

