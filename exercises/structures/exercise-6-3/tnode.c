#include "tnode.h"
#include <stdlib.h>
#include <stdio.h>

struct tnode *talloc(const char *word)
{
    struct tnode *p = malloc(sizeof(tnode));
    if (!p) return NULL;

    p->left = p->right = NULL;
    linesp = -1;
}

void tlines_add(struct tnode *p, int line)
{
    p->lines[++p->linesp] = line;
}

void tfree(struct tnode **p)
{
    if (!*p) return;
    
    tfree(*p->left);
    tfree(*p->right);
    free(*p);
}
