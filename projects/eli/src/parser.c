
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
    np->type = NODE_BINARY
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
    Token t = p->tokens[p->pos++];
    if (t.type == TOKEN_NEWLINE)
        p->line++;
    return t;
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
    p->err = 1;
    recover(p);
}

static inline void
unexpected(Parser *p, char c)
{
    fprintf(stderr, "Syntax Error: Unexpected '%c' at line %zu\n", c, p->line);
    p->err = 1;
    recover(p);
}

static inline void
recover(Parser *p)
{
    while (peek(p).type != TOKEN_SEMICOLON && peek(p).type != TOKEN_NULL)
        advance(p);
    if (peek(p).type == TOKEN_SEMICOLON)
        advance(p);
}

static inline astnode *
make_assignment(char *ident, astnode *val)
{
    astnode *np = malloc(sizeof(astnode));
    if (!np) allocfail();
    np->type = NODE_ASSIGNMENT;
    np->assign.name = ident;
    np->assign.value = val;
    return np;
}

static inline astnode *
make_err(void)
{
    astnode *np = malloc(sizeof(astnode));
    if (!np) allocfail();
    np->type = NODE_ERROR;
    return np;
}

static inline const char *
toktostr(Token t)
{
    switch (t.type)
    {
    case TOKEN_IDENT: return "identifier";
    case TOKEN_INT: return "integer";
    case TOKEN_DOUBLE: return "double";
    case TOKEN_STRING: return "string";

    case TOKEN_EQUAL: case TOKEN_PLUS: case TOKEN_MINUS:
    case TOKEN_STAR: case TOKEN_FSLASH: case TOKEN_LPAREN:
    case TOKEN_RPAREN:
        return (char *)t.c;
    default:
        return "idk";
    }
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
    astnode *np;
    Token t = advance(p);
    switch (t.type)
    {
        case TOKEN_IDENT:
            np = malloc(sizeof(astnode));
            if (!np) allocfail();

            np->type = NODE_IDENT; np->ident.name = t.ident;
            break;

        case TOKEN_INT:
            np = malloc(sizeof(astnode));
            if (!np) allocfail();

            np->type = NODE_NUMBER_INT;
            np->i = t.i;
            break;

        case TOKEN_DOUBLE:
            np = malloc(sizeof(astnode));
            if (!np) allocfail();
           
            np->type = NODE_NUMBER_DOUBLE;
            np->d = t.d;
            break;

        case TOKEN_LPAREN:
            np = parse_expression(p, 0.0);
            if (!match(p, TOKEN_RPAREN))
            {
                expect(p, ')');
                return make_err();
            }
            advance(p);
            break;
        default:
            unexpected(p, t.c);
            return make_err();
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
        if (bp < minbp) break;

        advance(p);
        right = parse_expression(p, bp + 0.1);
        left = make_binary(left, op, right);
    }

    return left;
}

astnode *
parse_assignment(Parser *p)
{
    Token ident = advance(p);
    Token eq = advance(p);

    if (eq.type != TOKEN_EQUAL)
        expect(p, '=');
    astnode *rhs = parse_expression(p, 0.0);

    Token sc = advance(p);
    if (sc.type != TOKEN_SEMICOLON)
        expect(p, ';');
    return make_assignment(ident.ident, rhs);
}
