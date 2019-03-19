/**
 * brainfuck_ops.c
 * Brainfuck interpreter written in C: this file hosts the functions
 * used to map Brainfuck operations into their C equivalents.
 * Source: https://github.com/eindiran/urbane
 */

/** Includes */
#include "brainfuck_ops.h"


/**
 * Increment a pointer: must pass the address.
 * Equivalent to the Brainfuck symbol '>'
 */
void increment_pointer(char **ptr) {
    #ifdef DEBUG
        fprintf(stderr, "Incrementing pointer: %p\n", *ptr); // DEBUG
    #endif
    (*ptr)++;
    #ifdef DEBUG
        fprintf(stderr, "New value: %p\n", *ptr); // DEBUG
    #endif
}


/**
 * Decrement a pointer: must pass the address.
 * Equivalent to the Brainfuck symbol '<'
 */
void decrement_pointer(char **ptr) {
    #ifdef DEBUG
        fprintf(stderr, "Decrementing pointer: %p\n", *ptr); // DEBUG
    #endif
    (*ptr)--;
    #ifdef DEBUG
        fprintf(stderr, "New value: %p\n", *ptr); // DEBUG
    #endif
}


/**
 * Increment pointer value.
 * Equivalent to the Brainfuck symbol '+'
 */
void increment_pointer_val(char *ptr) {
    #ifdef DEBUG
        fprintf(stderr, "Incrementing pointer value from: %c to: %c\n",
                *ptr, *ptr + 1); // DEBUG
    #endif
    (*ptr)++;
}


/**
 * Decrement pointer value.
 * Equivalent to the Brainfuck symbol '-'
 */
void decrement_pointer_val(char *ptr) {
    #ifdef DEBUG
        fprintf(stderr, "Decrementing pointer value from: %c to: %c\n",
                *ptr, *ptr - 1); // DEBUG
    #endif
    (*ptr)--;
}


/**
 * Output a character at ptr to STDOUT.
 * Equivalent to the Brainfuck symbol '.'
 */
void output_char_stdout(char *ptr) {
    #ifdef DEBUG
        fprintf(stderr, "Outputing char: %c\n", *ptr); // DEBUG
    #endif
    putchar(*ptr);
}


/**
 * Output a char at ptr to file `out`:
 * Specialized version of the symbol '.'
 */
void output_char_file(char *ptr, FILE *out) {
    #ifdef DEBUG
        fprintf(stderr, "Writing char %c to file.\n", *ptr); // DEBUG
    #endif
    fputc(*ptr, out);
}


/**
 * Set pointer value to a character grabbed from STDIN.
 * Equivalent to the Brainfuck symbol ','
 */
void input_char(char *ptr) {
    #ifdef DEBUG
        fprintf(stderr, "Inputting char from STDIN\n"); // DEBUG
    #endif
    *ptr = getchar();
}
