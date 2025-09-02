#ifndef STACK_H 
#define STACK_H 

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAX_SIZE 1000

typedef struct {
    char arr[MAX_SIZE];
    int top;
} Stack;

Stack *init_stack(); 
bool isEmpty(Stack *s);
bool isFull(Stack *s);
void popStack(Stack *s); 
void pushStack(Stack *s, char c); 
char peekStack(Stack *s); 

#endif 
