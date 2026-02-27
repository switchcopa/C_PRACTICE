#ifndef TNODE_H
#define TNODE_H 

#define MAX_LINES_APPEARANCE 512
#define WORD_SIZE 32

struct tnode
{
    int lines[MAX_LINES_APPEARANCE];
    char word[WORD_SIZE];
    struct tnode *left;
    struct tnode *right;
    int linesp;
};

struct tnode *talloc(const char *word);
void tlines_add(struct tnode *p, int line);
void tfree(struct tnode **p); // the caller should expect to pass the address of the pointer to the preffered node

#endif
