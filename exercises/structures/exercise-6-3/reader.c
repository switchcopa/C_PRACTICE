#include "reader.h"
#include <stdio.h>

int read_input(struct input_reader *r)
{
    r->bufp = -1;
    int c;

    while ((c = getchar()) != EOF && r->bufp < INPUT_BUFFER_SIZE - 1)
       r->buffer[++bufp] = c;

    if (c == EOF && r->bufp == -1)
        return -1;

    r->buffer[bufp + 1] = '\0';
    return bufp;
}

