#ifndef SYNTAX_CHECKER_H
#define SYNTAX_CHECKER_H

#include <stdio.h>
#include <stdbool.h>
#include "../stack/stack.h"

typedef enum {
    IN_NONE,
    IN_COMMENT,
    IN_LINE_COMMENT,
    IN_QUOTES,
    IN_DOUBLE_QUOTES
} State;

bool is_opening_brackets(char c);
bool is_closing_brackets(char c);
bool is_special_character(char c);
bool is_matching_brackets(char c_0, char c_1);
void report_close_brackets_error(char c, int line);
char *read_file_to_buffer(const char* filename, size_t *out_size);
void handle_brackets(Stack *s, char c, int line);
void handle_syntax_errors(char *file_content, size_t file_size);

#endif
