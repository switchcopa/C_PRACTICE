
/* lexer.h: tokenize input 
 * basically turn input into multiple
 * tokens that will be sent to the parser */

#ifndef _LEXER_H
#define _LEXER_H

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#define IDENTIFIER_SIZE 32
#define MAX_TOKENS 5000U
#define BUFSIZE 128

#define UNEXPECTED_SYNTAX_ERROR(c, line) (fprintf(stderr, "Syntax Error: Unexpected %c at line %zu\n", c , line))
#define valid_ident(c) (isalnum(c) || c == '_')

#define BIT(n)                    (1U << (n))
#define TOKEN_SET_FLAG(t, f)      ((t).flags |= (f))
#define TOKEN_CLEAR_FLAG(t, f)    ((t).flags &= ~(f))
#define TOKEN_HAS_FLAG(t, f)      ((t).flags & (f))
#define TOKEN_FLAG_KEYWORD        (BIT(0))
#define TOKEN_FLAG_OPERATOR       (BIT(1))
#define TOKEN_FLAG_CONSTANT       (BIT(2))
#define TOKEN_FLAG_ALLOCATED      (BIT(3))

typedef enum
{
    TOKEN_IDENT,
    TOKEN_EQUAL,
    TOKEN_INT,
    TOKEN_DOUBLE,
    TOKEN_STRING,
    TOKEN_PLUS,
    TOKEN_MINUS,
    TOKEN_STAR,
    TOKEN_FSLASH,
    TOKEN_LPAREN,
    TOKEN_RPAREN,
    TOKEN_ERROR,
    TOKEN_ALLOCERR,
    TOKEN_SEMICOLON,
    TOKEN_SPACE,
    TOKEN_NEWLINE,
    TOKEN_NULL,
    TOKEN_UNKNOWN,
    TOKEN_ELSE,
    TOKEN_FOR,
    TOKEN_IF,
    TOKEN_WHILE
} toktype;

typedef struct token
{
    toktype type;
    union
    {
        char ident[IDENTIFIER_SIZE];
        char *s;
        double d;
        int i;
        char c;
    };

    size_t line;
    unsigned char flags;
} Token;

typedef struct lexer
{
    Token *Tokens;
    size_t ntokens;
    size_t capacity;
    int err;
} Lexer;

typedef struct
{
    char *kw;
    toktype type;
} keyword;

Lexer *lex(char *buf);
extern Lexer _lexer;
extern size_t _line;
extern const char *toktypes[];
extern const keyword keywords[];

#endif
