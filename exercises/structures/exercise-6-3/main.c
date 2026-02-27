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

int main(void)
{
    struct tnode *tree;
    struct input_reader ir;
    if (read_input(&ir) < 0) {
        fprintf(stderr, "failed to read input\n");
        return EXIT_FAILURE;
    }

    for (;;)
    {
        char word[WORD_SIZE];
        char c;
        int i;

        for (i = 0; (c = ir.buffer[i]) != ' ' && (isalpha(c) || isdigit(c)) && i < WORD_SIZE - 1; i++)
            word[i] = c;
        word[i] = '\0';

        if (!noise_binsearch(word))
            
    }
}

static int noise_binsearch(const char *word)
{
    int left = 0;
    int right = sizeof(noise) / sizeof(noise[0]) - 1;

    while (left < right)
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
