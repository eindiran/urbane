/**
 * urbane.c
 * Brainfuck interpreter written in C: this file contains the main
 * interpreter code.
 * Source: https://github.com/eindiran/urbane
 */

/** Includes */
#include <unistd.h>
#include "urbane_utils.h"
#include "brainfuck_ops.h"


/**
 * Print out the usage of the program and exit.
 */
void print_usage() {
    fprintf(stderr, "Usage: urbane foo.bf        --> Interpret foo.bf using non-strict interpreter.\n");
    fprintf(stderr, "       urbane -s foo.bf     --> Interpret foo.bf using strict interpreter.\n");
    fprintf(stderr, "       urbane -t <N> foo.bf --> Interpret foo.bf, using <N> memory cells.\n");
    fprintf(stderr, "       urbane -h            --> Print this message and exit.\n");
    exit(0);
}


/**
 * This is the bulk of the interpreter logic.
 * Take in a memory_buffer and an instruction_buffer.
 */
int interpret_bf(char *instructions, int instruction_len, char *cptr, bool strict) {
    #ifdef DEBUG
        char *cell_ptr = cptr;
    #endif
    int instruction_ptr = 0;
    char current_instruction = instructions[instruction_ptr];
    
    while (instruction_ptr <= instruction_len) {
        
        // Allows whitespace in instructions:
        // Just skip over it
        if (isspace(current_instruction)) {
            current_instruction = instructions[++instruction_ptr];
            continue;
        }
        
        #ifdef DEBUG
            fprintf(stderr, "Current instruction: %c\n", current_instruction); // DEBUG
            fprintf(stderr, "instruction_ptr: %i\n", instruction_ptr); // DEBUG
        #endif
       
        switch (current_instruction) {
            case '>':
                increment_pointer(&cptr);
                break;
            case '<':
                decrement_pointer(&cptr);
                break;
            case '+':
                increment_pointer_val(cptr);
                break;
            case '-':
                decrement_pointer_val(cptr);
                break;
            case '.':
                output_char_stdout(cptr);
                break;
            case ',':
                input_char(cptr);
                break;
            case '[':
                // if *cptr isn't 0 continue to next instruction
                #ifdef DEBUG
                    fprintf(stderr, "cptr: %i\n", *cptr); // DEBUG
                #endif
                if (*cptr == 0) {
                    while ((current_instruction = instructions[++instruction_ptr]) != ']') {
                        if (instruction_ptr >= instruction_len) {
                            // Got all the way to the end
                            raise_error("Unmatched '[' found!");
                            return 1;
                        }
                    }
                }
                break;
            case ']':
                // if *cptr is 0 continue to next instruction
                #ifdef DEBUG
                    fprintf(stderr, "cptr: %i\n", *cptr); // DEBUG
                #endif
                if (*cptr != 0) {
                    while ((current_instruction = instructions[--instruction_ptr]) != '[') {
                        if (instruction_ptr <= 0) {
                            // Got all the way to the beginning
                            raise_error("Unmatched ']' found!");
                            return 1;
                        }
                    }
                }
                break;
            default:
                // Not whitespace or a valid Brainfuck symbol
                // When in strict mode, fail here
                if (strict) {
                    raise_error_instr("Non-Brainfuck character encountered in program instructions", current_instruction);
                    return 1;
                } else { // When not in strict mode, treat as whitespace instead
                    current_instruction = instructions[++instruction_ptr];
                    continue;
                }
        }
        current_instruction = instructions[++instruction_ptr];
        #ifdef DEBUG
           fprintf(stderr, "New instruction: %c\n", current_instruction); // DEBUG
        #endif
    }
    #ifdef DEBUG
        fprintf(stderr, "Final instruction pointer value: %d\n", instruction_ptr); //DEBUG
        fprintf(stderr, "Final cell pointer value: %d\n", (int)(cptr - cell_ptr)); //DEBUG
    #endif
    return 0;
}


/**
 * Main function:
 * Handles reading in the file + commandline options,
 * allocating/freeing memory, and calling the interpreter code in
 * interpret_bf()
 */
int main(int argc, char *argv[]) {

    bool strict_interpreter = false;
    int opt;
    extern int optind;
    extern char *optarg;
    char *filename;
    int tape_cells = 0;

    // Handle options
    while ((opt = getopt(argc, argv, "sht:")) != -1) {
        switch (opt) {
            case 's':
                strict_interpreter = true;
                break;
            case 'h':
                print_usage();
                break;
            case 't':
                tape_cells = atoi(optarg);
                break;
            default:
                fprintf(stderr, "Got unknown argument or option: %c\n", (char) opt);
                print_usage();
                break;
        }
    }

    if (tape_cells == 0) {
        tape_cells = TAPE_CELLS; // Use the preconfigured size (2 ^ 14)
    }
    #ifdef DEBUG
        fprintf(stderr, "Cells in memory tape: %d\n", tape_cells);
    #endif
    
    // Handle non-option arguments (filename)
    if (optind > argc) {
        raise_error("Did not receive a Brainfuck file to interpret.");
        print_usage();
    }
    filename = argv[optind];
    #ifdef DEBUG
        fprintf(stderr, "Proceeding with .bf file: %s\n", filename);
    #endif
    
    FILE *fp = fopen(filename, "r");

    long buff_size = get_instr_buff_size(fp, filename);
    if (buff_size == -1L) {
        exit(1);
    }
    
    char *instruction_buffer = alloc_tape(buff_size);
    char *memory_buffer = alloc_tape(tape_cells);
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
    #ifdef DEBUG
        printf_tape(instruction_buffer, (int) buff_size, false);
    #endif
    int retval = interpret_bf(instruction_buffer, (int) buff_size, memory_buffer, strict_interpreter);
    #ifdef DEBUG
        printf_tape(memory_buffer, 200, true);
    #endif
    free(instruction_buffer);
    free(memory_buffer);
    return retval;

}
