#include <stdio.h>
#include <stdlib.h>
#include "parser.h"
#include "lexer.h"

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

static void print_indent(int depth)
{
    for (int i = 0; i < depth; i++)
        printf("  ");
}

void print_ast(astnode *n, int depth)
{
    if (!n) return;

    print_indent(depth);

    switch (n->type)
    {
        case NODE_ASSIGNMENT:
            printf("Assignment (%s)\n", n->assign.name);
            print_ast(n->assign.value, depth + 1);
            break;

        case NODE_BINARY:
            printf("Binary (%s)\n", toktostr(n->binary.t.type));
            print_ast(n->binary.left, depth + 1);
            print_ast(n->binary.right, depth + 1);
            break;

        case NODE_IDENT:
            printf("Identifier (%s)\n", n->ident.name);
            break;

        case NODE_NUMBER_INT:
            printf("Int (%d)\n", n->i);
            break;

        case NODE_NUMBER_DOUBLE:
            printf("Double (%f)\n", n->d);
            break;

        case NODE_ERROR:
            printf("ERROR\n");
            break;

        default:
            printf("Unknown node\n");
            break;
    }
}

int
main(int argc, char **argv)
{
    if (argc != 2) { printf("usage\n"); exit(EXIT_FAILURE); }
    FILE *fp = fopen(argv[1], "r");
    if (!fp) { printf("failed to open file\n"); exit(EXIT_FAILURE); }

    fseek(fp, 0, SEEK_END);
    size_t size = ftell(fp);
    rewind(fp);

    char *buf = malloc(size + 1);
    if (fread(buf, sizeof(char), size, fp) < size)
    {
        printf("failed to read the file\n");
        fclose(fp);
        exit(EXIT_FAILURE);
    }

    buf[size] = '\0';

    printf("%s:\n", argv[1]);
    Parser *p = make_parser(buf);
    if (!p) { printf("parser error\n"); exit(EXIT_FAILURE); }

    while (p->pos < p->ntokens &&
           p->tokens[p->pos].type != TOKEN_NULL)
    {
        astnode *stmt = parse_assignment(p);
        if (p->err) {
            printf("parsing failed\n");
            break;
        }
        
        print_ast(stmt, 0);
        printf("\n");
    }

    exit(EXIT_SUCCESS);
}
