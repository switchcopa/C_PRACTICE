#ifndef NODE_H 
#define NODE_H 

#include <stdbool.h>

typedef struct node {
        struct node* next;
        char* key;
        char* value;
} node;

node* create_node(char* key, char* value);
bool delete_node(node** head, char* target_key); 
void insert_node(node** head, node* node_to_insert); 
char* find_node(node** head, char* value);

#endif
