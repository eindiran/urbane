# urbane
A [Brainfuck](https://esolangs.org/wiki/Brainfuck) interpreter written in C. Urbane is named for Brainfuck's author, [Urban Müller](https://esolangs.org/wiki/Urban_M%C3%BCller).

## Calling the interpreter
To use `urbane`, compile the interpreter using `make`:

```shell
make build
```

Then run the interpreter over a Brainfuck source code file:

```shell
./bin/urbane ./bf/foo.bf
```

### Interpreter options

* `-h`: Print out the usage information.
* `-s`: Interpret in strict mode: will mark non-whitespace characters that aren't in the Brainfuck spec as an error.
* `-t <N>`: Run the interpreter with a memory tape of size <N>. By default, <N> is 4096.

### Debug mode and embedded mode
You can compile a version of the interpreter intended for debugging using `make debug`. Call the debugging executable using `./bin/urbane-debug ./bf/foo.bf`.

Additionally, if you want a version of the interpreter that has been optimized for size (e.g. for embedded environments), compile the interpreter with `make embed`. The embedded interpreter executable will be found here: `./bin/urbane-embed`.

## What is Brainfuck?
Brainfuck is an [esoteric programming language](https://esolangs.org/wiki/Esoteric_programming_language); arguably the most famous and influential esolang, it is built with 8 simple commands and an instruction pointer. It is not intended for practical use as a programming language - instead it is an exercise in minimalism in the design of programming languages.

Brainfuck uses an infinite tape model of computation: there is an infinite array of memory cells, each initially set to zero. It is a cannonical example of a [Turing Tarpit](https://en.wikipedia.org/wiki/Turing_tarpit), meaning that it is [Turing complete](https://en.wikipedia.org/wiki/Turing_completeness) but is incredibly difficult to use when writing any non-trivial program.

Brainfuck is based on the formal programming language [P"](https://en.wikipedia.org/wiki/P%E2%80%B2%E2%80%B2). P" is the first imperative structured porgramming language which doesn't make use of goto (or an alias of goto) to be proved Turing Complete. Brainfuck is an expansion of P", adding the `,` and `.` commands discussed below.

### Commands

The following commands are valid commands usable in a Brainfuck program:
* `>`: Increment the pointer; move it one cell to the right.
* `<`: Decrement the pointer; move it one cell to the left.
* `+`: Increment the value in the memory cell under the pointer.
* `-`: Decrement the value in the memory cell under the pointer.
* `.`: Output the character signified by the value in the memory cell under the pointer. Usually this is interpreted as an ASCII value.
* `,`: Input a character (e.g. from STDIN) and stores it in the cell under the pointer.
* `[`: Jump past the matching `]` if the cell under the pointer is 0.
* `]`: Jump back to the matching `[` if the cell under pointer is non-zero.

### Example Programs
There are a number of example programs available in the `bf` directory.

_Cat_: This program is a brainfuck implementation of the `cat` program.

```brainfuck
,
[
    .,
]
```

It copies a character from STDIN to the first cell, then loops doing:
* Output contents of cell.
* Copy character from STDIN into cell.

This program makes use of only a single cell, never changing which cell the cell pointer is aimed at.

_Hello World!_: This program is a version of Hello World.

```brainfuck
++++++++
[
    >++++
    [
        >++
        >+++
        >+++
        >+
        <<<<-
    ]
    >+
    >+
    >-
    >>+
    [
        <
    ]
    <-
]
>>.
>---.+++++++..+++.
>>.
<-.
<.+++.------.--------.
>>+.
>++.
```

_Multiply 2 numbers_: This program takes in two integers from STDIN and multiplies them.
```brainfuck
,>,<
[
    >
    [
        ->+>+<<
    ]
    >>
    [
        -<<+>>
    ]
    <<<
    -
]
>>.
```

The pattern `,>,` is a common pattern in Brainfuck programs as it captures a value in the first cell, then moves to the second cell, and captures a second value there.
This program uses the first cell as a kinf of "for"-loop construct: while the value in the first cell is not zero, it adds cell 2 to cell 3 and decrements cell 1.

_Add 2 numbers_: This program takes in two integers from STDIN and adds them.
```brainfuck
,>,
[
    <+>-
]
<
------------------------------------------------
.
```

This program also uses the `,>,` construct to capture STDIN.
The main body of the program is the loop; the loop continually adds 1 to cell 1, while decrementing cell 2. When cell 2 is empty, the value in cell 1 is now the sum of cell 1 and cell 2.
Note that it subtracts 48 (ASCII '0') to print the numbers correctly.

_Echo input_: Reads in a single character from STDIN then echoes it to STDOUT. Because it continually advances the cell pointer to the right, the program will have undefined behavior once you've entered more characters than the number of memory cells in the tape.

```brainfuck
+[,.>+]
```

The pattern `[{expression}>+]` where `{expression}` is any Brainfuck expression that doesn't contain `<` will result in an infinite loop, until the memory tape has no remaining cells. In an ideal Brainfuck machine with an infinite tape, this is a true infinite loop.

## Source code
The source code for Urbane can be found on GitHub, [here](https://github.com/eindiran/urbane). It is licensed under the [MIT license](https://mit-license.org/).
