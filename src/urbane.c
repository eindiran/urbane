/**
 * urbane.c
 * Brainfuck interpreter written in C.
 * Source: https://github.com/eindiran/urbane
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>


/**
 * Increment a pointer.
 * Equivalent to the Brainfuck symbol '>'
 */
void increment_pointer(int *ptr) {
    ptr++;
}


/**
 * Decrement a pointer.
 * Equivalent to the Brainfuck symbol '<'
 */
void decrement_pointer(int *ptr) {
    ptr--;
}


/**
 * Increment pointer value.
 * Equivalent to the Brainfuck symbol '+'
 */
void increment_pointer_val(int *ptr) {
    ++*ptr;
}


/**
 * Decrement pointer value.
 * Equivalent to the Brainfuck symbol '-'
 */
void decrement_pointer_val(int *ptr) {
    --*ptr;
}


/**
 * Output a character at ptr to STDOUT.
 * Equivalent to the Brainfuck symbol '.'
 */
void output_char(int *ptr) {
    putchar(*ptr);
}


/**
 * Set pointer value to a character grabbed from STDIN.
 * Equivalent to the Brainfuck symbol ','
 */
void input_char(int *ptr) {
    *ptr = getchar();
}


/**
 *
 */
void raise_error(char *print_str, char instruction) {
    printf("ERROR: %s\n", print_str);
    printf("Instruction where error occurred: %c\n", instruction);
}


/**
 *
 */
int interpret_bf(char *instructions) {
    int instruction_ptr = 0;
    int current_instruction = instructions[instruction_ptr];
    while (current_instruction) {
        // Allows whitespace in instructions:
        // Just skip over it
        if (isspace(current_instruction)) {
            current_instruction = instructions[++instruction_ptr];
            continue;
        }
        switch (current_instruction) {
            case '>':
                increment_pointer(instruction_ptr);
                break;
            case '<':
                decrement_pointer(instruction_ptr);
                break;
            case '+':
                increment_pointer_val(instruction_ptr);
                break;
            case '-':
                decrement_pointer_val(instruction_ptr);
                break;
            case '.':
                output_char(instruction_ptr);
                break;
            case ',':
                input_char(instruction_ptr);
                break;
            case '[':
                // Add instructions to stack
                break;
            case ']':
                // Pop instructions off of stack
                break;
            default:
                // Not whitespace or a valid Brainfuck symbol
                raise_error("Non-Brainfuck character encountered in program instructions", current_instruction);
                return 1;
        }
    }
}


/**
 * Main function:
 * Handles reading in the file, allocating memory, and calling interpret_bf()
 */
int main(int argc, char *argv[]) {

    long buff_size;
    char *instruction_buffer;

    FILE *fp = fopen(argv[1], "r");
    // If we get a bad file pointer,
    // error out now
    if (!fp) {
        perror(argv[1]);
        exit(1);
    }

    // Find out how much we need to allocated for instruction_buffer
    fseek(fp, 0L, SEEK_END);
    buff_size = ftell(fp);
    fseek(fp, 0L, SEEK_SET); // Rewind file pointer

    // Allocate memory for instruction_buffer
    // Use calloc to ensure null-termination
    instruction_buffer = calloc(sizeof(char), buff_size);
    if (!instruction_buffer) {
        // Allocation failed
        fclose(fp);
        printf("Allocation failure\n");
        exit(1);
    }
    // Read file into instruction_buffer
    if (fread(instruction_buffer, buff_size, 1, fp) != 1) {
        fclose(fp);
        printf("Failure while reading in file\n");
        free(instruction_buffer);
        exit(1);
    }
    fclose(fp);
    int retval = interpret_bf(instruction_buffer);
    free(instruction_buffer);
    return retval;
}
