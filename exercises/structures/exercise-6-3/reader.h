#ifndef READER_H
#define READER_H

#define INPUT_BUFFER_SIZE 5024

#include <stddef.h>

struct input_reader
{
    char buffer[INPUT_BUFFER_SIZE];
    size_t buflen;
    int bufp;
};

int read_input(struct input_reader *r);

#endif
