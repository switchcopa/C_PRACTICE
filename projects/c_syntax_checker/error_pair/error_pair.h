#ifndef ERROR_PAIR_H
#define ERROR_PAIR_H

#include <stdbool.h>

typedef struct {
    int line;
    char symbol;
    bool closed;
} Pair;

Pair *makepair(int line, char symbol);
void setpair_line(Pair *pair, int line);

#endif
