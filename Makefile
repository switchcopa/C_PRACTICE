# ===========================
# Makefile
# ===========================

CC = gcc
CFLAGS = -g -std=c11 -Wall -Wextra -Wpedantic

ifndef FILE
$(error Please specify FILE, e.g., make FILE=example.c)
endif

OUT = $(basename $(FILE))

# Default: compile FILE into binary named after it
all:
	@echo "Compiling $(FILE) -> $(OUT)"
	$(CC) $(CFLAGS) $(FILE) -o $(OUT)

# Compile FILE into 'test'
test:
	@echo "Compiling $(FILE) -> test"
	$(CC) $(CFLAGS) $(FILE) -o test

# Compile FILE into 'debug'
debug:
	@echo "Compiling $(FILE) -> debug"
	$(CC) $(CFLAGS) $(FILE) -o debug

clean:
	rm -f $(wildcard *.o) $(wildcard *.out) $(wildcard *_bin) program test debug
	@echo "Removed binaries"
