#include "error_pair.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

Pair* makepair(int line, char symbol) {
    Pair* pair = malloc(sizeof(Pair));

    if (!pair) {
        printf("failed to allocate memory for pair\n");
        exit(1);
    }

    pair->line = line;
    pair->symbol = symbol;
    pair->closed = false;

    return pair;
}

void setpair_line(Pair *pair, int line) {
    pair->line = line;
}
