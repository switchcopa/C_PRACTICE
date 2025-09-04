#include <stdio.h> 
#include <stdlib.h>
#include "../stack/stack.h"
#include "syntax_checker.h"
#include "../error_pair/error_pair.h"

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

void handle_syntax_errors(char *file_content, size_t file_size) {
    Stack *s = init_stack();
    int c, i, nl, prev;
    State state = IN_NONE;

    prev = c = nl = 0;

    for (i = 0; i < file_size; i++) {
        c = file_content[i];

        if (c == '\n') {
            nl++;
            if (state == IN_LINE_COMMENT)
                state = IN_NONE;  
            prev = c;
            continue;
        }

        if (state == IN_BLOCK_COMMENT) {
            if (prev == '*' && c == '/')
                state = IN_NONE;   // close */
            prev = c;
            continue;
        }

        if (state == IN_SINGLE_QUOTE || state == IN_DOUBLE_QUOTE) {
            if (c == '\\') { 
                i++;
                prev = 0;
                continue;
            }
            if ((state == IN_SINGLE_QUOTE && c == '\'') ||
                (state == IN_DOUBLE_QUOTE && c == '"')) {
                state = IN_NONE; 
            }
            prev = c;
            continue;
        }

        if (prev == '/' && c == '/')
            state = IN_LINE_COMMENT;
        else if (prev == '/' && c == '*')
            state = IN_BLOCK_COMMENT;

        else if (c == '\'')
            state = IN_SINGLE_QUOTE;
        else if (c == '"')
            state = IN_DOUBLE_QUOTE;

        else if (c == '(' || c == '{' || c == '[') {
            Pair *p = malloc(sizeof(Pair));
            p->symbol = c;
            p->line = nl + 1;

            pushStack(s, p);
        } else if (c == ')' || c == '}' || c == ']') {
            if (isEmpty(s)) {
                printf("Syntax error: unexpected '%c' at line %d\n", c, nl + 1);
                exit(1);
            }

            Pair *top = peekStack(s);
            if (is_matching_brackets(top->symbol, c))  {
                popStack(s);
            } else {
                printf("Syntax error: mismatched '%c' at line %d (opened with '%c' at line %d)\n",
                       c, nl + 1, top->symbol, top->line);
                exit(1);
            }
        }

        prev = c;
    }

    if (state == IN_SINGLE_QUOTE)
        printf("Syntax error: unclosed single quote starting before line %d\n", nl + 1);
    else if (state == IN_DOUBLE_QUOTE)
        printf("Syntax error: unclosed double quote starting before line %d\n", nl + 1);
    else if (state == IN_BLOCK_COMMENT)
        printf("Syntax error: unclosed block comment before line %d\n", nl + 1);

    while (!isEmpty(s)) {
        Pair *top = peekStack(s);
        printf("Syntax error: unclosed '%c' opened at line %d\n", top->symbol, top->line);
        popStack(s); 
    }
    
    for (i = 0; i <= file_size; i++) {
        free(s->arr[i]);        
    }
}
