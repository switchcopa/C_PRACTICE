#ifndef STACK_H 
#define STACK_H 

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../error_pair/error_pair.h"

#define MAX_SIZE 1000

typedef struct {
    Pair* arr[MAX_SIZE];
    int top;
} Stack;

Stack *init_stack(); 
bool isEmpty(Stack *s);
bool isFull(Stack *s);
void popStack(Stack *s); 
void pushStack(Stack *s, Pair* pair); 
Pair* peekStack(Stack *s); 

#endif 
