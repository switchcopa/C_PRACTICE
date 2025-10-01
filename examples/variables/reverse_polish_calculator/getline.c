#include <stdio.h> 
#include "calc.h"

int get_line(char *line, unsigned int max_size) {
        int c, len;

        len = 0;
        while ((c = getchar()) != EOF && c != '\n') {
                if (len < max_size - 1) 
                        line[len++] = c;
        }

        line[len] = '\0';
        return len;
}
