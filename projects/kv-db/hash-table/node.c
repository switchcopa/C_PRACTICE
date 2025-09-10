#include "node.h" 
#include <stdio.h> 
#include <stdlib.h>
#include <string.h>

node* create_node(char* key, char* value) {
        node* new_node = malloc(sizeof(node));
        if (!new_node) {
                printf("failed to allocate memory for node\n");
                return NULL;
        }

        new_node->key = key;
        new_node->value = value;
        new_node->next = NULL;

        return new_node;
}

void delete_node(node** head, char* target_key) {
        if (!(*head)) return;

        if (strcmp((*head)->key, target_key)) { 
                (*head) = (*head)->next;
                return;
        }

        while(*head && (*head)->next) {
                if (strcmp((*head)->key, target_key)) { 
                        (*head)->next = (*head)->next->next; 
                }
        }
}

void insert_node(node** head, node* node_to_insert) {
        if (*head == NULL) {
                (*head) = node_to_insert;
                return;
        }
        
        node_to_insert->next = *head;
        (*head) = node_to_insert;
}
