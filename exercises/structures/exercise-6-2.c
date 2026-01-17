
/*
Write a program that reads a C program and prints in alphabetical order each
group of variable names that are identical in the first 6 characters, but different somewhere
thereafter. Don't count words within strings and comments. Make 6 a parameter that can be set
from the command line.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdint.h>

#define BUFFER_SIZE     1000
#define MAX_GROUP_SIZE  256
#define WORD_SIZE       32
#define DEFAULT_N_CHARS 6

struct tnode
{
    char *prefix;
    char **list_words;
    struct tnode *left;
    struct tnode *right;
};

struct tnode *talloc(void);
void addtree(struct tnode *, char *);
int strsuf(const char * restrict, const char * restrict);

static char input_buf[BUFFER_SIZE];
static char bufp = 0;
static uint64_t nchars = DEFAULT_N_CHARS;
char getch(void);
void ungetch(char);
void terminate(void);

int main(void)
{

    return EXIT_SUCCESS;
}

struct tnode *talloc(void)
{
    struct tnode *p = malloc(sizeof(struct tnode));
    if (!p)
        return NULL;

    p->list_words = malloc(sizeof(char *) * MAX_GROUP_SIZE);
    if (!p->list_words)
    {
        free(p);
        return NULL;
    }

    for (int i = 0; i < MAX_GROUP_SIZE; i++)
        p->list_words[i] = NULL;
    p->prefix = NULL;
    p->left = p->right = NULL;
    return p;
}

char getch(void)
{
    return (bufp > 0) ? input_buf[--bufp] : getchar();
}

void ungetch(char c)
{
    if (bufp >= BUFFER_SIZE)
    {
        fprintf(stderr, "attempt to push to full buffer\n");
        terminate();
    } else
        input_buf[bufp++] = c;
}

// if the root node is NULL, add a new node with the word w, if the word w is less than rquired
// N words, skip it, else add the first characters as a new prefix to the tree
// first, before appending it to the tree, traverse the tree by comparing to the right and left
// nodes suffixes lexicographically, if there is any, so strcmp(p->prefix, word) and if < 0
//
// begin with the root node:
void addtree(struct tnode *p, char *w)
{
    uint64_t len = strlen(w);
    if (len < nchars)
        return;

    if (p == NULL)
    {
        p = talloc();
        if (!p)
            return;
    }

    int cond;
    struct tnode *np = p;
    while (np)
    {
        if ()
    }
}

int strsuf(const char *restrict str, const char *restrict suffix)
{
    while (*suffix != '\0' && *str != '\0' && *str++ == *suffix++)
        ;

    if (*suffix == '\0')
        return 0;
    else return 1;
}
