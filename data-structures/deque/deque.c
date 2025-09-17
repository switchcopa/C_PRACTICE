#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#include "deque.h"

deque* create_deque(size_t size) {
        deque* dq = malloc(sizeof(deque));
        if (!dq) return NULL;

        dq->buffer = malloc(sizeof(uint8_t) * size);
        if (!dq->buffer) return NULL;

        dq->capacity = size;
        dq->size = 0;
        dq->front = dq->rear = -1;

        return dq;
}

bool is_empty(deque* dq) {
        if (!dq) return false;
        
        return dq->size == dq->capacity;
}

bool is_full(deque* dq) {
        if (!dq) return false;
        
        return ((rear + 1) % capacity == front);
}

void push_front(deque* dq, uint8_t data) {
        if (!dq || is_full(dq)) return;
        
        dq->front = (front - 1 + capacity) % capacity; 
        dq->buffer[dq->front] = data;
        dq->size++;
}

void push_back(deque* dq, uint8_t data) {
        if (!dq || is_full(dq)) return;
        
        dq->buffer[dq->rear] = data;
        dq->rear = (rear + 1) % capacity;
        dq->size++;
}

uint8_t pop_front(deque* dq) {
        if (!dq || is_empty(dq)) return 0;
        
        dq->size--;
        return dq->buffer[dq->front--];
}

uint8_t pop_back(deque* dq) {
        if (!dq || is_empty(dq)) return 0;
        
        dq->size--;
        return dq->buffer[dq->rear--];
}

void print_deque(deque* dq) {
        if (!dq) {
                printf("deque is NULL\n");
                return;
        }

        else if (is_empty(dq)) {
                printf("deque is empty\n");
                return;
        }

        printf("[ ");
        for (int i = dq->front; i < dq->size; i = (i+1) % capacity) 
                printf("%u ", dq->buffer[i]);

        printf("]\n");
}
