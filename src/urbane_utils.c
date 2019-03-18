/**
 * urbane_utils.c
 * Brainfuck interpreter written in C: this file provides a number
 * of utility functions for the main interpreter.
 * Source: https://github.com/eindiran/urbane
 */

/** Includes */
#include "urbane_utils.h"

/**
 * Print out error information.
 */
void raise_error(char *print_str) {
    fprintf(stderr,"ERROR: %s\n", print_str);
}


/**
 * Print out error information, including instruction where
 * the error occured.
 */
void raise_error_instr(char *print_str, char instruction) {
    raise_error(print_str);
    fprintf(stderr, "Instruction where error occurred: %c\n", instruction);
}


/**
 * Prints out the values in a tape. Useful for debugging.
 */
void printf_tape(char *tape, int tape_len, bool ints) {
    fprintf(stderr, "Tape: [");
    for (int i = 0; i < tape_len; i++) {
        if (ints) {
            fprintf(stderr, "'%i', ", (int) tape[i]);
        } else {
            fprintf(stderr, "'%c', ", tape[i]);
        }
    }
    fprintf(stderr, "]\n");
}


/**
 * Allocate a "memory tape" structure of size tape_len.
 * Suitable for allocation of the memory_buffer or the
 * instruction_buffer.
 */
char* alloc_tape(int tape_len) {
    char *memory_tape = calloc(sizeof(char), tape_len);
    if (memory_tape != NULL) {
        return memory_tape;
    } else {
        raise_error("Failed to allocate sufficient memory for memory_tape\n");
    }
    return NULL;
}


/**
 * Load the .bf file from a file pointer, reading it into the instruction_buffer.
 * Handles cleanup if the process fails at any point.
 * Closes the file pointer on success.
 */
void fill_instruction_buffer(char *instruction_buffer, FILE *input_file_fp, char *filename, char *memory_buffer, long buff_size) {
    if (!input_file_fp) {
        perror(filename);
        free(instruction_buffer);
        free(memory_buffer);
        exit(1);
    }
    if (fread(instruction_buffer, buff_size, 1, input_file_fp) != 1) {
        fclose(input_file_fp);
        fprintf(stderr, "Failure while reading in file %s.\n", filename);
        free(instruction_buffer);
        free(memory_buffer);
        exit(1);
    }
    fclose(input_file_fp);
}


/**
 * Get the size of the instruction_buffer.
 * Rewinds the file pointer once complete.
 */
long get_instr_buff_size(FILE *input_file_fp, char *filename) {
    long buff_size;
    if (fseek(input_file_fp, 0L, SEEK_END) != 0) {
        fprintf(stderr, "Couldn't fseek to end of file %s\n", filename);
        return -1L;
    }
    buff_size = ftell(input_file_fp);
    // Rewind the file pointer
    if (fseek(input_file_fp, 0L, SEEK_SET)) {
        fprintf(stderr, "Couldn't rewind to beginning of file %s\n", filename);
        return -1L;
    } else {
        return buff_size;
    }
}


/**
 * Does clean up of allocated arrays, then exits.
 */
void cleanup(char *instruction_buffer, char *memory_buffer) {
    free(instruction_buffer);
    free(memory_buffer);
    exit(1);
}
