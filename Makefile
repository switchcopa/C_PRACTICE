# ===========================
# Compile individual example/exercise files only
# Flags: -g -std=c11 -Wall -Wextra -Wpedantic
# ===========================

# Compiler
CC = gcc

# Compilation flags
CFLAGS = -g -std=c11 -Wall -Wextra -Wpedantic

# Source file must be provided
ifndef FILE
$(error Please specify FILE, e.g., make FILE=example.c)
endif

# Automatically name output after the source file (without .c)
OUT = $(basename $(FILE))

# ===========================
# Targets
# ===========================

# Default target: compile the FILE
all:
	@echo "Compiling $(FILE) -> $(OUT)"
	$(CC) $(CFLAGS) $(FILE) -o $(OUT)

# Compile as test program
test:
	@echo "Compiling $(FILE) -> test"
	$(CC) $(CFLAGS) $(FILE) -o test

# Compile as debug program
debug:
	@echo "Compiling $(FILE) -> debug"
	$(CC) $(CFLAGS) $(FILE) -o debug

# Clean binaries
clean:
	rm -f $(wildcard *.o) $(wildcard *.out) $(wildcard *_bin)
	@echo "Removed temporary and binary files"

# ===========================
# Usage:
# make FILE=example.c       	 -> compile a single example into binary 'example'
# make test FILE=example.c  	 -> compile a single example into binary 'test'
# make debug FILE=example.c 	 -> compile a single example into binary 'debug'
# make FILES="ex1.c ex2.c ex3.c" -> compile multiple files
# make clean                	 -> remove temporary/binary files
# ===========================
