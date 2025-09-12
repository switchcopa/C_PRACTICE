#include "node.h" 
#include <stdio.h> 
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAX_KEY_VALUE_SIZE 20

node* create_node(char* key, char* value) {
        node* new_node = malloc(sizeof(node));
        if (!new_node) {
                printf("failed to allocate memory for node\n");
                return NULL;
        }

        new_node->key = malloc(sizeof(char) * MAX_KEY_VALUE_SIZE);
        strcpy(new_node->key, key);
        new_node->value = malloc(sizeof(char) * MAX_KEY_VALUE_SIZE);
        strcpy(new_node->value, value);
        new_node->next = NULL;

        return new_node;
}

bool delete_node(node** head, char* target_key) {
        if (!(*head)) return false;

        if (strcmp((*head)->key, target_key) == 0) { 
                (*head) = (*head)->next;
                return true;
        }

        while(*head && (*head)->next) {
                if (strcmp((*head)->key, target_key) == 0) { 
                        (*head)->next = (*head)->next->next; 
                        return true;
                }
        }

        return false;
}

void insert_node(node** head, node* node_to_insert) {
        if (node_to_insert == NULL) return;

        if (*head == NULL) {
                (*head) = node_to_insert;
                return;
        }
        
        node_to_insert->next = *head;
        (*head) = node_to_insert;
}

char* find_node(node** head, char* key) {
        if (!(*head)) return NULL;

        while(*head && (*head)->next) {
                if (strcmp((*head)->key, key) == 0) { 
                        return (*head)->value;        
                }
        }

        return NULL;
}
