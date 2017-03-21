# File:   Makefile for 3D FFT on CPU.
# Author: Aydin Arik

# Definitions.
TARGET = filter
FILE = filterPCFixed
CC = gcc
CFLAGS = -Wall
DEL = rm


# Default target.
all: $(TARGET)


# Compile: create object files from C source files.
filter.o: $(FILE).c fixedptc.h
	$(CC) -c $(CFLAGS) $< -o $@

# Link: create an run file.
$(TARGET): $(FILE).o
	$(CC) $(CFLAGS) $^ -o $@


# Target: clean project.
.PHONY: clean
clean: 
	$(DEL) -f *.o *.out
