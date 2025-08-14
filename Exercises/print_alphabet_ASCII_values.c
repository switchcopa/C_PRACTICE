#include <stdio.h>

int main() {
    int a;

    a = 65;
    while (a <= 90) {
        printf("The ASCII value of the letter %c is: %d\n", a, a);
        a++;
    }

    return 0;
}
