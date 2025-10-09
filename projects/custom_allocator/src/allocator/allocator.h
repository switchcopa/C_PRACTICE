#ifndef CUSTOM_ALLOCATOR_H
#define CUSTOM_ALLOCATOR_H

#include <stddef.h>

struct mem_block {
        struct mem_block *next_block;
        size_t block_size;
        int free;
};

void            init_allocator(void);
void*           zalloc(size_t size);
void            zfree(void *ptr);

#endif
