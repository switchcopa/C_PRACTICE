#include <stdio.h>

#define IN 1
#define OUT 0
#define MAX_LENGTH 20

int main() {
    int c, i, state, word_length;
    int len_count[MAX_LENGTH];

    for (i = 0; i < MAX_LENGTH; i++)
        len_count[i] = 0;
    
    state = OUT;
    word_length = 0;
    while ((c = getchar()) != EOF) {
        if (state == OUT && c != ' ' && c != '\n' && c != '\t') {
            state = IN;
            word_length++;
        }

        else if (state == IN && (c == ' ' || c == '\n' || c == '\t')) {
            state = OUT;
            len_count[word_length]++;
            word_length = 0;
        }
    }
    
    for (i = 0; i < MAX_LENGTH; i++) {
        if (len_count[i] == 0) continue;

        printf("There are %d words that are %d letters long.\n", len_count[i], i);
    }

    return 0;
}
