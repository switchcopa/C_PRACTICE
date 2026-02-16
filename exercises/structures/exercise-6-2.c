
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

#define BUFFER_SIZE      1000
#define MAX_GROUP_SIZE   256
#define WORD_SIZE        32
#define DEFAULT_N_CHARS  6
#define TNODE_QUEUE_SIZE 256

struct tnode
{
    char *prefix;
    char **list_words;
    struct tnode *left;
    struct tnode *right;
};

struct tnode_queue
{
    struct tnode *q[TNODE_QUEUE_SIZE];
    int rear;
    int front;
};

struct tnode *talloc(void);
void addtree(struct tnode *, char *);
int strsufix(const char * restrict, const char * restrict);

void init_tnode_queue(struct tnode_queue *q);
void is_empty(struct tnode_queue *q);
void enqueue(struct tnode_queue *q, struct tnode *node);
struct tnode *dequeue(struct tnode_queue *q);

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

struct tnode *talloc(char *w)
{
    if (!w)
        return NULL;
    struct tnode *p = malloc(sizeof(struct tnode));
    if (!p)
        return NULL;

    p->list_words = malloc(sizeof(char *) * MAX_GROUP_SIZE);
    p->prefix = malloc(sizeof(char) * (nchars + 1));
    if (!p->list_words || !p->prefix)
    {
        free(p);
        return NULL;
    }
    
    strcpy(p->list_words[0], w); // make sure to make w fit into p->list_words[0]
    for (int i = 1; i < MAX_GROUP_SIZE; i++)
        p->list_words[i] = NULL;
    
    strncpy(p->prefix, w, nchars);
    p->prefix[nchars] = '\0';
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

void addtree(struct tnode *p, char *w)
{
    if (!w)
        return;
    
    if (strlen(w) < nchars)
        return;

    if (p == NULL)
    {
        p = talloc(w);
        if (!p)
            return;
    }
    
    int found = 0;
    struct tnode_queue q; 
    init_tnode_queue(&q);
    enqueue(&q, p);

    struct tnode *np;
    while (!is_empty(&q))
    {
        np = dequeue(&q);
        if (strprefix(np->prefix, w))
            for (int i = 0; i < MAX_GROUP_SIZE; i++)
                if (p->list_words[i] == NULL)
                {
                    p->list_words[i] = w;
                    found = 1;
                    break;
                }

        if (np->left) enqueue(&q, np->left);
        if (np->right) enqueue(&q, np->right);
    }
    
    if (found) return;
    if (!np->right)
    {
        np->right = talloc(w);
        if (!np->right) return;
        
                
    }
}

int strprefix(const char *restrict str, const char *restrict prefix)
{
    while (*prefix != '\0' && *str != '\0' && *str++ == *prefix++)
        ;

    if (*prefix == '\0')
        return 1;
    else return 0;
}

void init_tnode_queue(struct tnode_queue *q)
{
    q->rear  = -1;
    q->front = 0;
}

void is_empty(struct tnode_queue *q)
{
    return q->front > q->rear;
}

void enqueue(struct tnode_queue *q, struct tnode *node)
{
    q->q[++q->rear] = node;
}

struct tnode *dequeue(struct tnode_queue *q)
{
    return q->q[q->front++];
}
