#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include "reader.h"
#include "tnode.h"

static const char *noise[] = {
	"a","an","and","are","as","at","be","but","by","for","from","had","has","have",
	"he","her","him","his","i","in","is","it","its","of","on","or","that","the",
	"to","was","were","with","you"
};

static int noise_binsearch(const char *word);
static void getword(char *dest, const char *src, int *p, char *c);
static void treeprint(struct tnode *tree);

int main(void)
{
    struct tnode *tree = NULL;
    struct input_reader ir;
    if (read_input(&ir) < 0) {
        fprintf(stderr, "failed to read input\n");
        return EXIT_FAILURE;
    }

    int line = 0;
    for (int i = 0; i < ir.bufp; i++)
    {
        char word[WORD_SIZE], c;
        getword(word, ir.buffer, &i, &c);
        if (word[0] != '\0' && !noise_binsearch(word))
            tree = tree_add(tree, word, line);

        if (c == '\n') line++;
        else if (c == '\0') break;
    }

    treeprint(tree);
    tfree(tree);
}

static int noise_binsearch(const char *word)
{
    int left = 0;
    int right = sizeof(noise) / sizeof(noise[0]) - 1;

    while (left <= right)
    {
        int middle = (left + right) / 2;
        int cmp = strcmp(word, noise[middle]);

        if (cmp == 0)
            return 1;
        if (cmp < 0)
            right = middle - 1;
        else
            left = middle + 1;
    }

    return 0;
}

static char *getword2(const char * restrict src, int *bufp);

static void getword(char *dest, const char *src, int *p, char *c)
{
    int i = *p, j = 0;
    while (src[i] != '\0' && !isalnum((unsigned char)src[i++]))
        ;
    while (src[i] != '\0' && isalnum((unsigned char)src[i]))
        dest[j++] = src[i++];
    dest[j] = '\0';

    *c = src[i];
    *p = (src[i] == '\0') ? i : i + 1;
}

static void treeprint(struct tnode *tree)
{
    if (!tree) return;

    treeprint(tree->left);
    printf("%s: ", tree->word);
    for (int i = 0; i < tree->linesp; i++)
        printf("%d ", tree->lines[i]);
    printf("\n");

    treeprint(tree->right);
}
