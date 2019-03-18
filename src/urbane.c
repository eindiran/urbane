/**
 * urbane.c
 * Brainfuck interpreter written in C: this file contains the main
 * interpreter code.
 * Source: https://github.com/eindiran/urbane
 */

/** Includes */
#include <unistd.h>
#include "urbane_utils.h"

/**
 * Increment a pointer.
 * Equivalent to the Brainfuck symbol '>'
 */
void increment_pointer(char *ptr) {
    fprintf(stderr, "Incrementing pointer: %x\n", ptr); // DEBUG
    ptr = ptr + 1;
}


/**
 * Decrement a pointer.
 * Equivalent to the Brainfuck symbol '<'
 */
void decrement_pointer(char *ptr) {
    fprintf(stderr, "Decrementing pointer: %x\n", ptr); // DEBUG
    ptr = ptr - 1;
}


/**
 * Increment pointer value.
 * Equivalent to the Brainfuck symbol '+'
 */
void increment_pointer_val(char *ptr) {
    // fprintf(stderr, "Incrementing pointer value from: %c to: %c\n", *ptr, *ptr + 1); // DEBUG
    *ptr = *ptr + 1;
}


/**
 * Decrement pointer value.
 * Equivalent to the Brainfuck symbol '-'
 */
void decrement_pointer_val(char *ptr) {
    // fprintf(stderr, "Decrementing pointer value from: %c to: %c\n", *ptr, *ptr - 1); // DEBUG
    *ptr = *ptr - 1;
}


/**
 * Output a character at ptr to STDOUT.
 * Equivalent to the Brainfuck symbol '.'
 */
void output_char(char *ptr) {
    // fprintf(stderr, "Outputing char:\n"); // DEBUG
    putchar(*ptr);
}


/**
 * Set pointer value to a character grabbed from STDIN.
 * Equivalent to the Brainfuck symbol ','
 */
void input_char(char *ptr) {
    // fprintf(stderr, "Inputting char:\n"); // DEBUG
    *ptr = getchar();
}


/**
 * This is the bulk of the interpreter logic.
 * Take in a memory_buffer and an instruction_buffer.
 */
int interpret_bf(char *instructions, int instruction_len, char *cptr, bool strict) {
    int instruction_ptr = 0;
    char current_instruction = instructions[instruction_ptr];
    while (instruction_ptr <= instruction_len) {
        // fprintf(stderr, "Current instruction: %c\n", current_instruction); // DEBUG
        // fprintf(stderr, "instruction_ptr: %i\n", instruction_ptr); // DEBUG
        // Allows whitespace in instructions:
        // Just skip over it
        if (isspace(current_instruction)) {
            current_instruction = instructions[++instruction_ptr];
            continue;
        }
        switch (current_instruction) {
            case '>':
                cptr++;
                break;
            case '<':
                cptr--;
                break;
            case '+':
                *cptr = *cptr + 1;
                break;
            case '-':
                *cptr = *cptr - 1;
                break;
            case '.':
                output_char(cptr);
                break;
            case ',':
                input_char(cptr);
                break;
            case '[':
                // if *cptr isn't 0 continue to next instruction
                // fprintf(stderr, "cptr: %i\n", *cptr); // DEBUG
                if (*cptr == 0) {
                    while ((current_instruction = instructions[++instruction_ptr]) != ']') {
                        if (instruction_ptr >= instruction_len) {
                            // Got all the way to the end
                            raise_error("Unmatched '[' found!");
                            exit(1);
                        }
                        break;
                    }
                    continue;
                }
                break;
            case ']':
                // if *cptr is 0 continue to next instruction
                // fprintf(stderr, "cptr: %i\n", *cptr); // DEBUG
                if (*cptr != 0) {
                    while ((current_instruction = instructions[--instruction_ptr]) != '[') {
                        if (instruction_ptr <= 0) {
                            // Got all the way to the beginning
                            raise_error("Unmatched ']' found!");
                            exit(1);
                        }
                        break;
                    }
                    continue;
                }
                break;
            default:
                // Not whitespace or a valid Brainfuck symbol
                if (strict) {
                    raise_error_instr("Non-Brainfuck character encountered in program instructions", current_instruction);
                    return 1;
                } else {
                    current_instruction = instructions[++instruction_ptr];
                    continue;
                }
                break; // This is unreachable, but the linter complains if its not here.
        }
        current_instruction = instructions[++instruction_ptr];
        // fprintf(stderr, "New instruction: %c\n", current_instruction); // DEBUG
    }
    return 0;
}


/**
 * Test out printing the tape.
 * May be used as main if name is switched.
 */
int test_printf_tape(int argc, char *argv[]) {
    int ch_len = 20;
    // char *instruction_tape = alloc_tape(ch_len);
    char *memory_tape = alloc_tape(ch_len);
    char *cptr = memory_tape;
    for (int i = 0; i < ch_len; i++) {
        if (i % 3 == 0) {
            increment_pointer_val(cptr);
        }
        cptr++;
    }
    printf_tape(memory_tape, ch_len, true);
    return 0;
}


/**
 * Print out the usage of the program and exit.
 */
void print_usage() {
    fprintf(stderr, "Usage: urbane foo.bf    --> Interpret foo.bf using non-strict interpreter.\n");
    fprintf(stderr, "       urbane -s foo.bf --> Interpret foo.bf using strict interpreter.\n");
    fprintf(stderr, "       urbane -h        --> Print this message and exit.\n");
    exit(0);
}


/**
 * Main function:
 * Handles reading in the file, allocating memory, and calling interpret_bf()
 */
int main(int argc, char *argv[]) {

    bool strict_interpreter = false;
    int opt;
    extern int optind;
    char *filename;

    // Handle options
    while ((opt = getopt(argc, argv, "sh")) != -1) {
        switch (opt) {
            case 's':
                strict_interpreter = true;
                break;
            case 'h':
                print_usage();
                break;
            default:
                fprintf(stderr, "Got unknown argument or option: %c\n", (char) opt);
                print_usage();
                break;
        }
    }
    
    // Handle non-option arguments (filename)
    if (optind > argc) {
        raise_error("Did not receive a Brainfuck file to interpret.");
        print_usage();
    }
    filename = argv[optind];
    fprintf(stderr, "Proceeding with .bf file: %s\n", filename);
    
    FILE *fp = fopen(filename, "r");

    long buff_size = get_instr_buff_size(fp, filename);
    if (buff_size == -1L) {
        exit(1);
    }
    
    char *instruction_buffer = alloc_tape(buff_size);
    char *memory_buffer = alloc_tape(TAPE_CELLS);
    if (!instruction_buffer || !memory_buffer) {
        cleanup(instruction_buffer, memory_buffer); // Exit program
    }

    // If we get a bad file pointer, error out now
    if (!fp) {
        perror(argv[1]);
        cleanup(instruction_buffer, memory_buffer); // Exit program
    }

    // Read file into instruction_buffer
    if (fread(instruction_buffer, buff_size, 1, fp) != 1) {
        fclose(fp);
        raise_error("Failure while reading in file.");
        cleanup(instruction_buffer, memory_buffer); // Exit program
    }
    fclose(fp);
    printf_tape(instruction_buffer, (int) buff_size, false);
    int retval = interpret_bf(instruction_buffer, (int) buff_size, memory_buffer, strict_interpreter);
    printf_tape(memory_buffer, 200, true);
    free(instruction_buffer);
    free(memory_buffer);
    return retval;

}
