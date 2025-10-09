#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "allocator.h"

static unsigned char allocbuf[HEAP_SIZE]; // This is the "heap",
                                        // even though it's stored in initialized memory.
static struct mem_block *head_block = NULL;

// Run this before main, compile with GCC
__attribute__((constructor))
static void auto_init_allocator(void) {
        init_allocator();
}

void init_allocator(void) {
        head_block = (struct mem_block *)allocbuf; // make the first sizeof(struct mem_block *) bytes of allocbuf
                                                // those those of head_block

        head_block->next_block = NULL;
        head_block->free = 1;
        head_block->block_size = HEAP_SIZE - sizeof(struct mem_block *);
}

// If the requested size is enough, we create another block_size
// and return only the requested
static void split_block(struct mem_block *block, size_t size);

// Call this before freeing to reduce block fragmentation
// This memory pools large allocations
static void coalesce_block(void);

void *zalloc(size_t size);

void zfree(void *ptr);
