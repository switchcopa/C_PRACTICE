
/* parser.c */
#include "lexer.h"
#include "parser.h"
#include <stdlib.h>
#include <stdbool.h>

static inline void
allocfail(void)
{
    fprintf(stderr, "fatal! failed to allocate memory, exiting now\n");
    exit(EXIT_FAILURE);
}

static inline astnode *
make_binary(astnode *left, Token op, astnode *right)
{
    astnode *np = malloc(sizeof(astnode));
    if (!np) allocfail();
    np->binary.right = right;
    np->binary.left = left;
    np->binary.t = op;
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

static inline Token
peek_next(Parser *p)
{
    return p->tokens[p->pos + 1];
}

static inline bool
match(Parser *p, toktype tp)
{
    return (peek(p).type == tp);
}

static inline void
expect(Parser *p, char c)
{
    fprintf(stderr, "Syntax Error: Expected '%c' at line %zu\n", c, p->line);
    Token t;
    while ((t = advance(p)).type != TOKEN_SEMICOLON && t.type != TOKEN_NULL)
        ;
    if (t.type == TOKEN_NULL)
        exit(EXIT_FAILURE);
    else if (t.type == TOKEN_SEMICOLON)
        advance(p); // consume it
}

Parser *
make_parser(char *buf)
{
    Parser *p = malloc(sizeof(Parser));
    Lexer  *l = lex(buf);
    if (!p || !l) allocfail();

    p->tokens = l->Tokens;
    p->ntokens = l->ntokens;
    p->capacity = l->capacity;
    p->pos = 0U;
    p->line = 1U;
    p->err = 0;

    return p;
}

astnode *
parse_primary(Parser *p)
{
    astnode *np = malloc(sizeof(astnode));
    if (!np) allocfail();

    Token t = advance(p);
    switch (t.type)
    {
        case TOKEN_IDENT:
            np->type = NODE_IDENT; np->ident.name = t.ident;
            break;

        case TOKEN_INT:
            np->type = NODE_NUMBER_INT;
            np->i = t.i;
            break;

        case TOKEN_DOUBLE:
            np->type = NODE_NUMBER_DOUBLE;
            np->d = t.d;
            break;

        case TOKEN_LPAREN:
            free(np);
            np = parse_expression(p, 0.0);
            if (!match(p, TOKEN_RPAREN))
            {
                expect(p, ')');
                return NULL;
            }
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
    if (!left) return NULL;

    while (true)
    {
        Token op = peek(p);
        float bp = get_precedence(op.type);
        if (bp < minbp) break;

        advance(p);
        right = parse_expression(p, bp + 0.1);
        left = make_binary(left, op, right);
        if (!left) return NULL;
    }

    return left;
}

astnode *
parse_assignment(Parser *p)
{
    Token ident = advance(p);
    Token eq = advance(p);

    if (eq.type != TOKEN_EQUAL) { expect(p, '='); return NULL; };
    astnode *rhs = parse_expression(p, 0.0);
    if (!rhs) return NULL;

    Token sc = advance(p);
    if (sc.type != TOKEN_SEMICOLON)
        expect(p, ';');

    astnode *np = malloc(sizeof(astnode));

    if (!np) allocfail();
    np->type = NODE_ASSIGNMENT;
    np->assign.name = ident.ident;
    np->assign.value = rhs;
    return np;
}
