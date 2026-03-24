
/* parser.h: take input from lexer,
 * feed into struct Parser,
 * and pass it to AST
 */
#ifndef _PARSER_H
#define _PARSER_H

#include "lexer.h"
#include <stddef.h>

typedef struct
{
    Token *tokens;
    size_t ntokens;
    size_t capacity;
    size_t pos;
    int err;
} Parser;

void parse_expression(void);
void expect(toktype);

#endif
