
/* parser.h: take input from lexer,
 * feed into struct Parser,
 * and pass it to AST
 */
#ifndef _PARSER_H
#define _PARSER_H

#include "lexer.h"
#include <stddef.h>
#include <stdbool.h>

#define EXPECTED_ERROR(c, l) (fprintf(stderr, "Syntax Error: Expected '%c' at line %zu\n", c, l))

typedef enum
{
    NODE_NUMBER_DOUBLE,
    NODE_NUMBER_INT,
    NODE_BINARY,
    NODE_UNARY,
    NODE_IDENT,
    NODE_ASSIGNMENT,
    NOTE_STATEMENT_LIST
} node_type;

typedef struct
{
    Token *tokens;
    size_t ntokens;
    size_t capacity;
    size_t pos;
    size_t line;
    int err;
} Parser;

typedef struct astnode
{
    union
    {
        double d;
        int i;

        struct
        {
            struct astnode *right;
            struct astnode *left;
            Token t;
        } unary;

        struct
        {
            struct astnode *right;
            struct astnode *left;
            Token t;
        } binary;

        struct
        {
            char *name;
        } ident;

        struct
        {
            char *name;
            struct astnode *value;
        } assign;

        struct
        {
            astnode **stmts;
            size_t nstmt;
        } stmt_list;
    };
    node_type type;
} astnode;

astnode *parse_primary(Parser *p);
astnode *parse_expression(Parser *, float); // the float is the binding power of each token
astnode *parse_statement(Parser *);
astnode *parse_assignment(Parser *);
astnode *parse_program(Parser *);

#endif
