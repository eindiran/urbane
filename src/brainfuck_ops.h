/**
 * brainfuck_ops.h
 * Header for brainfuck_ops.c
 * Source: https://github.com/eindiran/urbane
 */
#include <stdio.h>


/** Functions */
void increment_pointer(char **ptr);

void decrement_pointer(char **ptr);

void increment_pointer_val(char *ptr);

void decrement_pointer_val(char *ptr);

void output_char_stdout(char *ptr);

void output_char_file(char *ptr, FILE *out);

void input_char(char *ptr);
