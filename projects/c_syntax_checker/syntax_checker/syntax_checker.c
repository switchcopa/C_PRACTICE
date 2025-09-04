#include <stdio.h> 
#include <stdlib.h>
#include "../stack/stack.h"
#include "syntax_checker.h"
#include "../error_pair/error_pair.h"

#define MAXSIZE 2000
bool has_errors = false;

bool is_opening_brackets(char c) {
    return c == '{' || c == '(' || c == '[';
}

bool is_closing_brackets(char c) {
    return c == '}' || c == ')' || c == ']';
}

bool is_special_character(char c) {
    return c == '\'' || c == '\"' || c == '\\';
}

bool is_matching_brackets(char c_0, char c_1) {
    return ((c_0 == '{' && c_1 == '}')) ||
            ((c_0 == '(' && c_1 == ')')) ||
            ((c_0 == '[' && c_1 == ']'));
}

void report_close_brackets_error(char c, int line) {
    printf("Syntax Error: Expected closing '%c' in line %d\n", c, line);
}

char *read_file_to_buffer(const char* filename, size_t *out_size) {
    FILE* fp = fopen(filename, "r");
    if (!fp) {
        perror("Failed to open file\n");
        exit(1);
    }

    fseek(fp, 0, SEEK_END);
    size_t size = ftell(fp);
    rewind(fp);

    char* buffer = malloc(size + 1);
    if (!buffer) {
        printf("failed to allocate memory for buffer\n");
        fclose(fp);
        exit(1);
    }

    fread(buffer, sizeof(char), size, fp);
    buffer[size] = '\0';
    fclose(fp);
    
    if (out_size) *out_size = size;
    return buffer;
}
