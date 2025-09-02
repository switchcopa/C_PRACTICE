#include <stdio.h>
#include <stdlib.h>
#include "stack.h"

Stack *init_stack() {
    Stack* s = malloc(sizeof(Stack));
    if (!s) { 
        printf("Failed to allocated memory for stack\n");
        exit(1);
    }

    s->top = -1;
    
    return s;
}

bool isEmpty(Stack *s) {
    return s->top == -1;
}

bool isFull(Stack *s) {
    return MAX_SIZE - 1 == s->top;
}

void popStack(Stack *s) {
    if (isEmpty(s)) {
        printf("cannot pop empty stack, stack underflow error\n");
        exit(1);
    }

    s->top--;
}

void pushStack(Stack *s, char c) {
    if (isFull(s)) {
        printf("cannot push to full stack, stack overflow error\n");
        exit(1);
    }

    s->arr[++s->top] = c;
}

char peekStack(Stack *s) {
    if (isEmpty(s)) {
        printf("cannot peek empty stack\n");
        exit(1);
    }
    
    return s->arr[s->top];
}
