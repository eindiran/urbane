# urbane
A [Brainfuck](https://esolangs.org/wiki/Brainfuck) interpreter written in C. Urbane is named for Brainfuck's author, [Urban Müller](https://esolangs.org/wiki/Urban_M%C3%BCller).

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
* `[`: Jump past the matching `]` is the cell under the pointer is 0.
* `]`: Jump back to the matching `[` is the cell under pointer is non-zero.

### Example Programs
There are a number of example programs available in the `bf` directory.

_Cat_: This program is a brainfuck implementation of the `cat` program.

```brainfuck
,
[
    .,
]
```

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

_Multiply 2 numbers_: This program takes in two integers and multiplies them.
```brainfuck
+++++
[
    -
    >+++
    <
]
>.
```

_Add 2 numbers_: This program takes in two integers and adds them.
```brainfuck
[
    -
    >+
    <
]
```

## Source code
The source code for Urbane can be found on GitHub, [here](https://github.com/eindiran/urbane).
