#include <stdio.h>
#include <stdlib.h>
#include "parser.h"
#include "lexer.h"

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
    fread(buf, sizeof char, size, fp);
    buf[size] = '\0';

    printf("%s:\n", argv[1]);
    Parser *p = make_parser(buf);
    if (!p) { printf("parser error\n"); exit(EXIT_FAILURE);
}
