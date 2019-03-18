/**
 * urbane_utils.h
 * Header for urbane_utils.c
 * Source: https://github.com/eindiran/urbane
 */

/** Includes */
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>


/** Defines */
#define TAPE_CELLS 16384 // 2 ^ 14

/** Functions */
void raise_error(char *print_str);
void raise_error_instr(char *print_str, char instruction);
void printf_tape(char *tape, int tape_len, bool ints);
char* alloc_tape(int tape_len);
void fill_instruction_buffer(char *instruction_buffer, FILE *input_file_fp, char *filename, char *memory_buffer, long buff_size);
long get_instr_buff_size(FILE *input_file_fp, char *filename);
void cleanup(char *instruction_buffer, char *memory_buffer);
