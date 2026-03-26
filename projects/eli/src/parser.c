
/* parser.c */
#include "lexer.h"
#include "parser.h"
#include <stdlib.h>
#include <stdbool.h>

static inline astnode *
make_binary(astnode *left, Token op, astnode *right)
{
    astnode *np = malloc(sizeof(astnode));
    if (!np) return NULL;
    np->binary.right = right;
    np->binary.left = left;
    np->binry.t = op;
    return np;
}

static inline float
get_precedence(toktype tp)
{
    switch (tp)
    {
        case TOKEN_PLUS:
        case TOKEN_MINUS:
            return 1.0;
        case TOKEN_STAR:
        case TOKEN_FSLASH:
            return 2.0;
        default:
            return 0.0;
    }
}

static inline Token
advance(Parser *p)
{
    return p->tokens[p->pos++];
}

static inline Token
peek(Parser *p)
{
    return p->tokens[p->pos];
}

static inline bool
match(Parser *p, toktype tp)
{
    return (peek(p).type == tp);
}

astnode *
parse_primary(Parser *p)
{
    astnode *np = malloc(sizeof(astnode));
    if (!np) return NULL;

    Token t = advance(p);
    switch (t.type)
    {
        case TOKEN_INT:
            np->type = NODE_NUMBER_INT;
            np->i = t.i;
            break;
        case TOKEN_DOUBLE:
            np->type = NODE_NUMBER_DOUBLE;
            np->d = t.d;
            break;
        case TOKEN_LPAREN:
            np = parse_expression(p, 0.0);
            if (!match(TOKEN_LPAREN))
                EXPECTED_ERROR(')', p->line);
            break;
        default:
            UNEXPECTED_SYNTAX_ERROR(t.c, p->line);
            return NULL;
    }

    return np;
}

astnode *
parse_expression(Parser *p, float minbp)
{
    astnode *left = parse_primary(p);
    astnode *right;

    while (true)
    {
        Token op = peek(p);
        float bp = get_precedence(op.type);
        if (bp < min_bp) break;

        advance();
        right = parse_expression(p, bp + 0.1);
        left = make_binary(left, op, right);
    }

    return left;
}
