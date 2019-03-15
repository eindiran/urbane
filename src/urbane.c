/**
 * urbane.c
 * Brainfuck interpreter written in C: this file contains the main
 * interpreter code.
 * Source: https://github.com/eindiran/urbane
 */

/** Includes */
#include "urbane_utils.h"

/**
 * Increment a pointer.
 * Equivalent to the Brainfuck symbol '>'
 */
void increment_pointer(char *ptr) {
    ptr++;
}


/**
 * Decrement a pointer.
 * Equivalent to the Brainfuck symbol '<'
 */
void decrement_pointer(char *ptr) {
    ptr--;
}


/**
 * Increment pointer value.
 * Equivalent to the Brainfuck symbol '+'
 */
void increment_pointer_val(char *ptr) {
    ++*ptr;
}


/**
 * Decrement pointer value.
 * Equivalent to the Brainfuck symbol '-'
 */
void decrement_pointer_val(char *ptr) {
    --*ptr;
}


/**
 * Output a character at ptr to STDOUT.
 * Equivalent to the Brainfuck symbol '.'
 */
void output_char(char *ptr) {
    putchar(*ptr);
}


/**
 * Set pointer value to a character grabbed from STDIN.
 * Equivalent to the Brainfuck symbol ','
 */
void input_char(char *ptr) {
    *ptr = getchar();
}


/**
 * This is the bulk of the interpreter logic.
 * Take in a memory_buffer and an instruction_buffer.
 */
int interpret_bf(char *instructions, char *cell_ptr) {
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
        current_instruction++;
    }
    return 0;
}


/**
 * Main function:
 * Handles reading in the file, allocating memory, and calling interpret_bf()
 */
int main(int argc, char *argv[]) {

    if (argc < 2) {
        raise_error("Did not receive a Brainfuck file to interpret.");
        fprintf(stderr, "Usage: ./urbane foo.bf\n");
    }

    char *filename = argv[1];
    fprintf(stderr, "Proceeding with .bf file: %s\n", filename);
    FILE *fp = fopen(filename, "r");

    long buff_size = get_instr_buff_size(fp, filename);
    if (buff_size == -1L) {
        exit(1);
    }
    
    char *instruction_buffer = alloc_tape(buff_size);
    char *memory_buffer = alloc_tape(TAPE_CELLS);
    if (!instruction_buffer || !memory_buffer) {
        cleanup(instruction_buffer, memory_buffer); // Exit program here
    }

    // If we get a bad file pointer, error out now
    if (!fp) {
        perror(argv[1]);
        cleanup(instruction_buffer, memory_buffer); // Exit program here
    }

    // Read file into instruction_buffer
    if (fread(instruction_buffer, buff_size, 1, fp) != 1) {
        fclose(fp);
        raise_error("Failure while reading in file.");
        cleanup(instruction_buffer, memory_buffer); // Exit program here
    }
    fclose(fp);
    int retval = interpret_bf(instruction_buffer, memory_buffer);
    free(instruction_buffer);
    free(memory_buffer);
    return retval;

}
