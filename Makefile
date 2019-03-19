# Urbane
# A simple Brainfuck interpreter written in C.
# Source: https://github.com/eindiran/urbane

CC = gcc
CCFLAGS = -Wall -fPIC -Ofast -c
# All warnings, position-independent code, use max optmizations,
# and wait to link.
DEBUGFLAGS = -Wall -Og -g3 -DDEBUG=1 -c
# All warnings, debug-level optimizations, maximum debug info,
# set DEBUG, and wait to link
EMBEDFLAGS = -Wall -Os -c
# All warnings, optimize for binary size, and wait to link.
INCLUDES = src/urbane_utils.h
CSOURCES = \
		   src/urbane.c \
		   src/urbane_utils.c \
		   src/brainfuck_ops.c

.PHONY: compile
compile:	$(CSOURCES)
	rm -f bin/urbane
	$(CC) $(CCFLAGS) -include $(INCLUDES) src/urbane_utils.c -o build/urbane_utils.o
	$(CC) $(CCFLAGS) src/brainfuck_ops.c -o build/brainfuck_ops.o
	$(CC) $(CCFLAGS) -include $(INCLUDES) src/urbane.c -o build/urbane.o
	$(CC) -O3 -Wall -o bin/urbane build/urbane.o build/urbane_utils.o build/brainfuck_ops.o
	@printf "\nSHA256 Checksum of executable: "
	@sha256sum bin/urbane


.PHONY: compile_debug
compile_debug:	$(CSOURCES)
	rm -f bin/urbane-debug
	$(CC) $(DEBUGFLAGS) -include $(INCLUDES) src/urbane_utils.c -o build/urbane_utils.o
	$(CC) $(DEBUGFLAGS) src/brainfuck_ops.c -o build/brainfuck_ops.o
	$(CC) $(DEBUGFLAGS) -include $(INCLUDES) src/urbane.c -o build/urbane.o
	$(CC) -Og -g3 -Wall -o bin/urbane-debug build/urbane.o build/urbane_utils.o build/brainfuck_ops.o
	@printf "\nSHA256 Checksum of executable: "
	@sha256sum bin/urbane-debug


.PHONY: compile_embed
compile_embed:	$(CSOURCES)
	rm -f bin/urbane-embed
	$(CC) $(EMBEDFLAGS) -include $(INCLUDES) src/urbane_utils.c -o build/urbane_utils.o
	$(CC) $(EMBEDFLAGS) src/brainfuck_ops.c -o build/brainfuck_ops.o
	$(CC) $(EMBEDFLAGS) -include $(INCLUDES) src/urbane.c -o build/urbane.o
	$(CC) -Os -Wall -o bin/urbane-embed build/urbane.o build/urbane_utils.o build/brainfuck_ops.o
	@printf "\nSHA256 Checksum of executable: "
	@sha256sum bin/urbane-embed


.PHONY: mkbin
mkbin:
	mkdir -p build/
	mkdir -p bin/


.PHONY: clean
clean:
	rm -rf build/*


.PHONY: build
build:	mkbin clean compile


.PHONY: debug
debug:	mkbin clean compile_debug


.PHONY: embed
embed:	mkbin clean compile_embed


.PHONY: all
all: build
