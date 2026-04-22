#include "lexer.h"
#include <stdio.h>
#include <stdlib.h>

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
    
    printf("%s:\n", argv[1]); 
    Lexer *lexer = lex(buf);
    for (size_t i = 0; i < lexer->ntokens; i++) {
        Token *t = &lexer->Tokens[i];
        if (t->type == TOKEN_NEWLINE) { fprintf(stdout, "\n"); continue; }
        fprintf(stdout, "%s ", toktypes[(int)t->type]);
    }
    
    printf("\n");
    exit(EXIT_SUCCESS);
}
