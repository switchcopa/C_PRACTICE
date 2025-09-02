#include <stdio.h> 
#include <stdlib.h>
#include <stdint.h>
#include "../stack/stack.h"
#include "syntax_checker.h"

#define MAXSIZE 2000

char f_buffer[MAXSIZE];

bool is_opening_brackets(char c) {
    return c == '{' || c == '(' || c == '[';
}

bool is_closing_brackets(char c) {
    return c == '}' || c == ')' || c == ']';
}

bool is_special_character(char c) {
    return c == '\'' || c == '\"' || c == '\\';
}

void report_close_brackets_error(char c, int line) {
    printf("Syntax Error: Expected closing '%c' in line %d\n", c, line);
    exit(1);
}

char *read_file_to_buffer(const char* filename) {
    FILE* fp = fopen(filename, "w");
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

    return buffer;
}

void handle_brackets(Stack* s, char c, int line) {
    if (is_opening_brackets(c)) {
        pushStack(s, c);
    }

    else if (is_closing_brackets(c)) {
        char t = peekStack(s);

        if (t != c) report_close_brackets_error(c, line);
        else popStack(s);
    }
}

void handle_special_characters(Stack *s, char c, int line) {
    if (is_special_character(c)) {
        char t = peekStack(s);

        if (is_special_character(t) && t == c) {
            popStack(s);
        }

        else report_close_brackets_error(c, line);
    }
}

void handle_syntax_errors(char *file_content, size_t file_size) {
    int c, ptr, nl; 
    Stack* s = init_stack();
    
    ptr = nl = 0;
    c = file_content[ptr];
    while ((c = file_content[ptr]) != '\0' && file_size > ptr) {
        if (c == '\n') {
            nl++;
            ptr++;
            continue;
        }

        handle_brackets(s, c, nl);
        handle_special_characters(s, c, nl);
        
        ptr++;
    }
}
