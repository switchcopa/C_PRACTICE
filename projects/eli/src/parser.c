
/* parser.c */
#include "lexer.h"
#include "parser.h"
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

static inline void
allocfail(void)
{
    fprintf(stderr, "fatal! failed to allocate memory, exiting now\n");
    exit(EXIT_FAILURE);
}

static inline char *
strdup(char *s)
{
    char *p = malloc(strlen(s) + 1);
    if (!p) allocfail();
    strcpy(p, s);
    return p;
}

static inline astnode *
make_binary(astnode *left, Token op, astnode *right)
{
    astnode *np = malloc(sizeof(astnode));
    if (!np) allocfail();
    np->type = NODE_BINARY;
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

static inline void
recover(Parser *p)
{
    while (peek(p).type != TOKEN_SEMICOLON && peek(p).type != TOKEN_NULL)
        advance(p);
    if (peek(p).type == TOKEN_SEMICOLON)
        advance(p);
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

static inline const char *
toktostr(toktype tp)
{
    switch (tp)
    {
    case TOKEN_IDENT: return "identifier";
    case TOKEN_INT: return "integer";
    case TOKEN_DOUBLE: return "double";
    case TOKEN_STRING: return "string";

    case TOKEN_EQUAL: return "=";
    case TOKEN_PLUS: return "+";
    case TOKEN_MINUS: return "-";
    case TOKEN_STAR: return "*";
    case TOKEN_FSLASH: return "/";
    case TOKEN_LPAREN: return "(";
    case TOKEN_RPAREN: return ")";
    case TOKEN_SEMICOLON: return ";";

    default: return "unknown";
    }
}

static inline void
expect(Parser *p, toktype tp)
{
    Token t = peek(p);
    fprintf(stderr, "Syntax Error: Expected '%s' at line %zu\n", toktostr(tp), t.line);
    p->err = 1;
    recover(p);
}

static inline void
unexpected(Parser *p, Token t)
{
    fprintf(stderr, "Syntax Error: Unexpected '%s' at line %zu\n", toktostr(t.type), t.line);
    p->err = 1;
    recover(p);
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

            np->type = NODE_IDENT; np->ident.name = strdup(t.ident);
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
                expect(p, TOKEN_RPAREN);
                return make_err();
            }
            advance(p);
            break;
        default:
            unexpected(p, t);
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
        if (bp == 0.0 || bp < minbp) break;

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
    if (ident.type != TOKEN_IDENT) { unexpected(p, ident); return make_err(); }
    Token eq = advance(p);
    if (eq.type != TOKEN_EQUAL)
        expect(p, TOKEN_EQUAL);

    astnode *rhs = parse_expression(p, 0.0);

    Token sc = advance(p);
    if (sc.type != TOKEN_SEMICOLON)
        expect(p, TOKEN_SEMICOLON);
    return make_assignment(ident.ident, rhs);
}
