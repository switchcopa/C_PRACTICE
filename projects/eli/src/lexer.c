
/* lexer.c */
#include "lexer.h"
#include <stdbool.h>
#include <ctype.h>
#include <string.h>
#include <stdio.h>

Lexer _lexer;
size_t _line;

static Token
get_num(char **buf)
{
    char *p = *buf;
    char num[BUFSIZE];
    Token t;
    int i;
    bool isfloat = false;

    for (i = 0; (*p == '.' || isdigit(*p)) && i < BUFSIZE - 1; i++)
    {
        if (*p == '.') {
            if (isfloat) {
                UNEXPECTED_SYNTAX_ERROR(*p, _line);
                t.type = TOKEN_ERROR;
                return t;
            } else
                isfloat = true;
        }

        num[i] = *p++;
    }

    num[i] = '\0';
    *buf = p;
    if (isfloat)
    {
        t.type = TOKEN_DOUBLE;
        t.d = atof(num);
    } else {
        t.type = TOKEN_INT;
        t.i = atoi(num);
    }

    return t;
}

static Token
get_ident(char **buf)
{
    char *p = *buf;
    Token t;
    int i;
    
    for (i = 0; valid_ident(*p) && i < IDENTIFIER_SIZE - 1; i++)
        t.ident[i] = *p++;
    t.ident[i] = '\0';
    if (valid_ident(*p))
    {
        fprintf(stderr, "Identifier name too long at line %zu\n", _line);
        t.type = TOKEN_ERROR;
        return t;
    }

    t.type = TOKEN_IDENT;
    *buf = p;
    return t;
}

static Token
get_string(char **buf)
{
    char *p = *buf + 1;
    char *s = malloc(sizeof(char) * BUFSIZE);
    Token t;
    if (!s)
    {
        fprintf(stderr, "failed to allocate memory\n");
        t.type = TOKEN_ALLOCERR;
        return t;
    }

    int i;
    for (i = 0; *p != '"' && i < BUFSIZE - 1; i++)
        s[i] = *p;
    s[i] = '\0';

    if (*p != '"') 
    {
        fprintf(stderr, "String too long at line %zu\n", _line);
        free(s);
        t.s = NULL;
        t.type = TOKEN_ERROR;
        return t;
    }
    
    t.type = TOKEN_STRING;
    t.s = s;
    *buf = ++p;
    return t;
}

static Token
get_symbol(char **buf)
{
    char *p = *buf;
    Token t;
    switch (*p)
    {
        case '=':
            t.type = TOKEN_EQUAL;
            break;

        case '+':
            t.type = TOKEN_PLUS;
            break;
        
        case '-':
            t.type = TOKEN_MINUS;
            break;

        case '*':
            t.type = TOKEN_STAR;
            break;

        case '/':
            t.type = TOKEN_FSLASH;
            break;

        case ')':
            t.type = TOKEN_RPAREN;
            break;
        
        case '(':
            t.type = TOKEN_LPAREN;
            break;

        case ' ':
        case '\t':
        case '\f':
        case '\v':
        case '\r':
            t.type = TOKEN_SPACE;
            break;

        case '\n':
            t.type = TOKEN_NEWLINE;
            break;

        case '\0':
            t.type = TOKEN_NULL;
            break;
        default:
            t.type = TOKEN_UNKNOWN;
            break;
    }
    
    t.c = *p++;
    *buf = p;
    return t;
}

Token
next_token(char **buf)
{
    char *p = *buf;
    if (isalpha(*p)) return get_ident(buf);
    if (isdigit(*p)) return get_num(buf);
    if (*p == '"') return get_string(buf);

    return get_symbol(buf);
}

int
lexer_add_token(Token t)
{
    if (_lexer.ntokens < _lexer.capacity)
        _lexer.Tokens[_lexer.ntokens++] = t;
    else {
        Token *nlex_tokens = realloc(_lexer.Tokens, _lexer.capacity * 2);
        if (!nlex_tokens) return 0;
        _lexer.capacity *= 2;
        _lexer.Tokens = nlex_tokens;
        _lexer.Tokens[_lexer.ntokens++] = t;
    }

    return 1;
}

Lexer*
lex(char *buf)
{
    Token t;
    _lexer.capacity = MAX_TOKENS;
    if (!(_lexer.Tokens = malloc(sizeof(Token) * _lexer.capacity))) goto allocerr;

    while ((t = next_token(&buf)).type != TOKEN_NULL)
    {
        if (t.type == TOKEN_ALLOCERR) goto allocerr;
        else if (t.type == TOKEN_ERROR) // skip all tokens until a safe point
        {
            lexer_add_token(t);
            while ((t = next_token(&buf)).type != TOKEN_NEWLINE)
                ;
        }
        else if (t.type == TOKEN_NEWLINE) _line++;
        else if (t.type == TOKEN_UNKNOWN)
            fprintf(stderr, "error: unknown character token %c\n", t.c);

        if (!lexer_add_token(t)) goto allocerr;
    }
    
    lexer_add_token(t);
    return &_lexer;
allocerr:
    fprintf(stderr, "fatal error: failed to allocate memory\nexiting now...\n");
    exit(EXIT_FAILURE);
}
