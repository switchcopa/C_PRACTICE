#include <stdio.h>

int main() {
    int a;

    a = 33;
    while (a <= 126) {
        printf("The ASCII value of the letter %c is: %d\n", a, a);
        a++;
    }

    return 0;
}
