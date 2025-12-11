#include <stdio.h>

int main() {
    // This is a line comment

    char c = '\'';   // single quote inside char
    char d = '"';   // double quote inside char

    /*
       This is a block comment
      nested-looking comment */

    if (1) {   // correct brackets
        int arr[3] = {1, 2, 3};
    }

    if (0) {   // missing closing bracket - should trigger error
        printf(Oops!);
    
    printf("Escaped \" quotes \n still works.n");

    return 0;
	}
}
