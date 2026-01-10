#include <stdio.h>
#include <ctype.h>
#include <string.h>

#define MAX_WORD 100

int getword(char *, int);
int binsearch(char *, struct key *, int);

struct key
{
    char *word;
    int count;
} keytab[] = {
    { "auto", 0 },
    { "break", 0 },
    { "case", 0 },
    { "char", 0 },
    { "const", 0 },
    { "continue", 0 },
    { "default", 0 },
    { "for", 0 },
    { "signed", 0 },
    { "unsigned", 0 },
    { "void", 0 },
    { "volatile", 0 },
    { "while", 0 }
};

#define N_KEYS (sizeof keytab / sizeof(struct key))

int main()
{
    int c;
    char word[MAX_WORD];

    while (getword(word, MAX_WORD) != EOF)
        if (isalpha(word[0]))
            if ((n = binsearch(word, keytab, N_KEYS)) >= 0)
                keytab[n].count++;

    for (n = 0; n < N_KEYS; n++)
        if (keytab[n].count > 0)
            printf("%4d %s\n", keytab[n].count, keytab[n].word);

    return 0;
}

int binsearch(char *word, struct key tab[], int n)
{
    int cond;
    int low = 0;
    int high = n - 1;

    while (low <= high)
    {
        int mid = (low + high) / 2;
        if ((cond = strcmp(word, tab[mid].word)) < 0)
            high = mid - 1;
        else if (cond > 0)
            low = mid + 1;
        else
            return mid;
    }

    return -1;
}
