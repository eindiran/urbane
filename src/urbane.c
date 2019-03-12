/**
 * urbane.c
 * Brainfuck interpreter written in C.
 * Source: https://github.com/eindiran/urbane
 */

// Includes
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>


// Defines
#define TAPE_CELLS 4096



/**
 *
 */
void raise_error(char *print_str) {
    printf("ERROR: %s\n", print_str);
}


/**
 *
 */
void raise_error_instr(char *print_str, char instruction) {
    raise_error(print_str);
    printf("Instruction where error occurred: %c\n", instruction);
}


/**
 *
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
int interpret_bf(char *instructions, char *cell_ptr,
                 long instruction_ptr) {
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
                increment_pointer(cell_ptr);
                break;
            case '<':
                decrement_pointer(cell_ptr);
                break;
            case '+':
                increment_pointer_val(cell_ptr);
                break;
            case '-':
                decrement_pointer_val(cell_ptr);
                break;
            case '.':
                output_char(cell_ptr);
                break;
            case ',':
                input_char(cell_ptr);
                break;
            case '[':
                // Add instructions to stack
                break;
            case ']':
                // Pop instructions off of stack
                break;
            default:
                // Not whitespace or a valid Brainfuck symbol
                raise_error_instr("Non-Brainfuck character encountered in program instructions", current_instruction);
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
    long instruction_ptr = 0L;
    long cell_ptr = 0L;
    
    // Find out how much we need to allocated for instruction_buffer
    fseek(fp, 0L, SEEK_END);
    buff_size = ftell(fp);
    fseek(fp, 0L, SEEK_SET); // Rewind file pointer

    char *instruction_buffer = alloc_tape(buff_size);
    char *memory_buffer = alloc_tape(TAPE_CELLS);
    if (!instruction_buffer || !memory_buffer) {
        goto cleanup_and_exit;
    }

    FILE *fp = fopen(argv[1], "r");
    // If we get a bad file pointer,
    // error out now
    if (!fp) {
        perror(argv[1]);
        goto cleanup_and_exit;
    }

    // Read file into instruction_buffer
    if (fread(instruction_buffer, buff_size, 1, fp) != 1) {
        fclose(fp);
        printf("Failure while reading in file\n");
        goto cleanup_and_exit;
    }
    fclose(fp);
    int retval = interpret_bf(instruction_buffer);
    free(instruction_buffer);
    free(memory_buffer);
    return retval;

    cleanup_and_exit:
        free(instruction_buffer);
        free(memory_buffer);
        exit(1);
}
