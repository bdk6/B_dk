/* ***********************************************************************
 * @file codegen.h
 * @copy Copyright 2022 William R Cooke
 * @brief Handles code generation for virtual B machine.
 * ******************************************************************** */

word gen_initialize();
word gen_finish();

word gen_startDataBlock();
word gen_addDataWord(word d);
word gen_endDataBlock();

word gen_startBSSBlock();
word gen_addBSSWord(word d);
word gen_endBSSBlock();


word gen_if_start();
word gen_if_else();
word gen_if_end();
word gen_while_start();
word gen_while_exp();
word gen_while_end(word start_addr, word exp_addr);
word gen_function_call(word addr);
word gen_function_return();
word gen_goto(word addr);

word gen_get_parameter(word offset);
word gen_get_auto(word offset);
word gen_get_extrn(word addr);

word gen_add();
word gen_subtract();
word gen_multiply();
word gen_divide();
word gen_modulo();
word gen_and();
word gen_or();
word gen_xor();
word gen_negate();
word gen_complement();

