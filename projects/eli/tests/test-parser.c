#include <stdio.h>
#include <stdlib.h>
#include "parser.h"
#include "lexer.h"

void print_ast(astnode *node) {
    if (!node) return;

    switch (node->type) {
        case NODE_NUMBER_INT:
            printf("%d", node->i);
            break;
        case NODE_NUMBER_DOUBLE:
            printf("%f", node->d);
            break;
        case NODE_IDENT:
            printf("%s", node->ident.name);
            break;
        case NODE_BINARY:
            printf("(");
            print_ast(node->binary.left);
            printf(" %c ", node->binary.t.c); // or t.type mapping
            print_ast(node->binary.right);
            printf(")");
            break;
        case NODE_ASSIGNMENT:
            printf("%s = ", node->assign.name);
            print_ast(node->assign.value);
            printf(";");
            break;
        default:
            printf("<?>");
    }
}

int main(int argc, char **argv) {
    if (argc != 2) {
        fprintf(stderr, "usage\n");
        exit(EXIT_FAILURE);
    }

    FILE *fp = fopen(argv[1], "r");
    if (!fp) {
        fprintf(stderr, "failed to open file\n");
        exit(EXIT_FAILURE);
    }

    fseek(fp, 0, SEEK_END);
    size_t size = ftell(fp);
    rewind(fp);

    char *buf = malloc(size + 1);
    if (!buf) {
        fprintf(stderr, "alloc fail\n");
        fclose(fp);
        exit(EXIT_FAILURE);
    }

    if (fread(buf, sizeof(char), size, fp) < size)
    {
        fprintf(stderr, "error occured while reading file\n");
        fclose(fp);
        exit(EXIT_FAILURE);
    }
    buf[size] = '\0';

    Parser *p = make_parser(buf);
    if (!p) exit(EXIT_FAILURE);

    astnode *np = parse_assignment(p);
    if (np->type == NODE_ASSIGNMENT) {
        print_ast(np);
    } else 
        printf("not an assignment??\n");

    printf("\n");
    exit(EXIT_SUCCESS);
}
