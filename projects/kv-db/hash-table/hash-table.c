#include <stdio.h> 
#include "hash-table.h"
#include "node.h"
#include <string.h>
#include <stdlib.h>
#include <stdbool.h> 

h_table* create_table() {
        int i;
        h_table* table = malloc(sizeof(h_table));
        if (!table) {
                printf("failed to allocate memory for table\n");
                exit(1);
        }
        
        for (i = 0; i < MAX_TABLE_SIZE; i++) { 
                table->buckets[i] = NULL;
        }

        table->num_of_elements = 0;
        return table;
}

unsigned long hash(char* key) {
        unsigned long hash_val = 5381;
        int c;

        while ((c = *key++)) {
                hash_val = ((hash_val >> 5) + hash_val) + c;
        }

        return hash_val % MAX_TABLE_SIZE;
}

bool is_valid_character(char c) {
        return (c <= 'Z' && c >= 'A') || (c >= 'a' && c <= 'z') || (c >= '0' && c <= '9');
}

bool ht_create_entry(h_table* table, char *key, char *value) {
        if (!table) {
                printf("hash table is NULL\nexiting now...\n"); 
                exit(1);
        }

        int i, k_len, v_len;
        
        k_len = strlen(key);
        for (i = 0; i < k_len && i < MAX_KEY_VALUE_SIZE; i++) {
                if (!is_valid_character(key[i])) {
                        printf("Error: key contains invalid character \'%c\'\nPlease enter key with alphabet characters or numbers\n", key[i]);
                        return false;
                }
        }
        
        v_len = strlen(value);
        for (i = 0; i < v_len && i < MAX_KEY_VALUE_SIZE; i++) {
                if (!is_valid_character(value[i])) {
                        printf("Error: value contains invalid character \'%c\'\nPlease enter value with alphabet characters or numbers\n", value[i]);
                        return false;
                }
        }
        
        node* new_node = create_node(key, value);
        unsigned int hash_val = hash(key);
        
        insert_node(&(table->buckets[hash_val]), new_node);
        table->num_of_elements++;

        return true;
}

bool ht_delete_entry(h_table* table, char* key) {
        if (!table) {
                printf("hash table is NULL\nexiting now...\n");
                exit(1);
        }
        
        unsigned hash_val = hash(key);
        bool succ = delete_node(&table->buckets[hash_val], key);

        if (succ) printf("successfully deleted entry!\n");
        else printf("failed to delete entry, key doesn't exist\n");

        return succ;
}
