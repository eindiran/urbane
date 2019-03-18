# Urbane
# A simple Brainfuck interpreter written in C.
# Source: https://github.com/eindiran/urbane

CC = gcc
CCFLAGS = -Wall -fPIC -Ofast -std=gnu11 -c
# All warnings, position-independent code, use max optmizations, use GNU11 standard, don't link
DEBUGFLAGS = -Wall -v -fPIC -Og -g3 -std=gnu11 -c
# All warnings, verbose compiler output, position-independent code, debug-level optimizations, maximum debug info, use GNU11 standard, don't link.
EMBEDFLAGS = -Wall -Os -c
# All warnings, optimize for binary size, don't link.
INCLUDES = src/urbane_utils.h
CSOURCES = \
		   src/urbane.c \
		   src/urbane_utils.c

.PHONY: compile
compile:	$(CSOURCES)
	$(CC) $(CCFLAGS) -include $(INCLUDES) src/urbane_utils.c -o bin/urbane_utils.o
	$(CC) $(CCFLAGS) -include $(INCLUDES) src/urbane.c -o bin/urbane.o
	$(CC) -O3 -Wall -o bin/urbane bin/urbane.o bin/urbane_utils.o


.PHONY: compile_debug
compile_debug:	$(CSOURCES)
	$(CC) $(DEBUGFLAGS) -include $(INCLUDE) src/urbane_utils.c bin/urbane_utils.o
	$(CC) $(DEBUGFLAGS) -include $(INCLUDE) src/urbane.c bin/urbane.o
	$(CC) -O3 -Wall -o bin/urbane bin/urbane.o bin/urbane_utils.o


.PHONY: mkbin
mkbin:
	mkdir -p bin/


.PHONY: clean
clean:
	rm -rf bin/*


.PHONY: build
build:	clean mkbin compile


.PHONY: all
all: build
