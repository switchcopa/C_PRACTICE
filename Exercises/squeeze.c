#include <stdio.h>
#include <stdbool.h>

void squeeze(char s[], char t[]);

int main(void) {
    char s[6] = "hello\0";
    char t[5] = "wrld\0";

    squeeze(s, t);

    printf("%s\n", s);

    return 0;
}

void squeeze(char s[], char t[]) {
    int i, j, k; 
    bool to_remove;

    for (i = k = 0; s[i] != '\0'; i++) {
        to_remove = false;

        for (j = 0; t[j] != '\0'; j++) {
            if (s[i] == t[j]) {
                to_remove = true;
                break;
            }
        }

        if (!to_remove) s[k++] = s[i];
    }

    s[k] = '\0';
}
