#ifndef READER_H
#define READER_H

#define INPUT_BUFFER_SIZE 5024

struct input_reader
{
    char buffer[INPUT_BUFFER_SIZE];
    int bufp;
};

int read_input(struct input_reader *r);

#endif
