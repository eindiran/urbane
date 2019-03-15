# Urbane
# A simple Brainfuck interpreter written in C.
# Source: https://github.com/eindiran/urbane

CC = gcc
CCFLAGS = -Wall -O3 -c
DEBUGFLAGS = -Wall -g3 -c
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


.PHONY: clean
clean:
	rm -rf bin/*
	rm ./*.gc??
	rm ./*.o


.PHONY: build
build:	clean compile


.PHONY: all
all: build
