
/* lexer.h: tokenize input 
 * basically turn input into multiple
 * tokens that will be sent to the parser */

#ifndef _LEXER_H
#define _LEXER_H

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#define IDENTIFIER_SIZE 32
#define MAX_TOKENS 5000
#define BUFSIZE 128

#define UNEXPECTED_SYNTAX_ERROR(c, line) (fprintf(stderr, "Syntax Error: Unexpected %c at line %zu\n", c , line))
#define valid_ident(c) (isalnum(c) || c == '_')
#define lexer_add_token(l, t) (l->Tokens[l->ntokens++] = t)

extern size_t line;

typedef struct token
{
    enum {
        TOKEN_IDENT,
        TOKEN_EQUAL,
        TOKEN_INT,
        TOKEN_DOUBLE,
        TOKEN_STRING,
        TOKEN_PLUS,
        TOKEN_MINUS,
        TOKEN_MULT,
        TOKEN_DIV,
        TOKEN_LPAREN,
        TOKEN_RPAREN,
        TOKEN_ERROR,
        TOKEN_ALLOCERR,
        TOKEN_SPACE,
        TOKEN_NEWLINE,
        TOKEN_NULL
    } type;

    union {
        char ident[IDENTIFIER_SIZE];
        char *s;
        int i;
        double d;
        char c;
    };
} Token;

typedef struct lexer
{
    Token *Tokens;
    size_t ntokens;
} Lexer;

Lexer *lexer_tokenize(char *ibuf);

#endif
