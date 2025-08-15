#include <stdio.h>
    
#define IN 1
#define OUT 0
#define MAX_WORDS 1000

int main() {
    int c, i, state;
    int nwords[MAX_WORDS];
    
    for (i = 0; i < MAX_WORDS; i++) 
        nwords[i] = 0;
    
    state = OUT; 
    i = 0;
    while ((c = getchar()) != EOF) {
        if (c != ' ' && c != '\n' && c != '\t' && state == OUT) {
            state = IN;
        }
        
        else if ((c == ' ' || c == '\n' || c == '\t') && state == IN) {
            state = OUT;
            i++;
        }

        if (state == IN) {
            nwords[i]++;
        }
    }

    for (i = 0; i < MAX_WORDS && nwords[i] != 0; i++) {
            printf("%d ", nwords[i]);
    }

    printf("\n");
}
