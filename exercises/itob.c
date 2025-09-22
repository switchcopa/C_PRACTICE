#include <stdio.h> 
#include <string.h>
#include <stdlib.h>

void itob(int n, char s[]);
void reverse(char s[]);

int main(void) {
        int n = 166;
        char s[20];

        itob(n, s);
        printf("test hexadecimal: %x\n", n);
        printf("string: %s\n", s);

        return 0;
}

void itob(int n, char s[]) {
        if (s[0] == '\0') return;
        int i, remainder, sign;
        unsigned int num;

        if ((sign = num) < 0) 
                num = (unsigned) -n;

        do {
                remainder = num / 16;

                if (remainder < 10) 
                        s[i++] = (char) remainder;
                else 
                        s[i++] = (char) 'F' - remainder + 5;

        } while ((num /= 16) > 0);

        s[i] = '\0';
        reverse(s);
}

void reverse(char s[]) {
        int i, j;
        char temp;

        for (i = 0, j = strlen(s) - 1; i < j ; i++, j--) {
                temp = s[i];
                s[i] = s[j];
                s[j] = temp;
        }
}