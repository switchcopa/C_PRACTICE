#include <stdio.h> 
#include "hash-table.h"
#include "node.h"
#include <string.h>
#include <stdlib.h>
#include <stdbool.h> 

#define MAX_BUFFER_SIZE 10000

h_table* create_table(size_t size) {
        int i;
        h_table* table = malloc(sizeof(h_table));
        if (!table) {
                printf("failed to allocate memory for table\n");
                exit(1);
        }
        
        for (i = 0; i < MAX_TABLE_SIZE; i++) { 
                table->buckets[i] = NULL;
        }
        
        table->size = size;
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

char* ht_get_entry(h_table* table, char* key) {
        if (!table) {
                printf("hash table is NULL\nexiting now...\n");
                exit(1);
        }
        
        unsigned int hash_val = hash(key);
        char* val = find_node(&table->buckets[hash_val], key);

        if (!val) {
                printf("failed to get key \"%s\", key doesn't exist\n", key);
                return NULL;
        }

        return val; 
}

float ht_lfactor(h_table *table) {
        return (float) table->num_of_elements / (float) table->size;
}

void ht_rehash(h_table** table_ref) {
        if (!(*table_ref)) {
                printf("failed to rehash table, table is NULL\nexiting now...\n");
                exit(1);
        }

        int i, h_v; 
        bool succ;
        node* n_head;
        h_table* new_table = create_table((*table_ref)->size);
        if (!new_table) {
                printf("failed to rehash table\n");
                return;
        }

        for (i = 0; i < (*table_ref)->size; i++) {
                n_head = (*table_ref)->buckets[i];

                for (; n_head == NULL || n_head->next == NULL;) {
                        h_v = hash(n_head->key);
                        succ = ht_create_entry(new_table, n_head->key, n_head->value);

                        if (!succ) printf("rehash warning: failed to map entry to new table\n");
                }
        }

        printf("successfully rehashed table\n");
}

h_table* ht_open(const char* ht_filename) {
        FILE* fp = fopen(ht_filename, "r");
        if (!fp) {
                printf("failed to open file \"%s\"\n", ht_filename);
                return NULL;
        }

        h_table* table = create_table(MAX_TABLE_SIZE);
        if (!table) {
                printf("failed to load table from file \"%s\"\n", ht_filename);
                return NULL;
        }
        
        int i, j, k;
        bool in_word;
        size_t len, f_len;
        unsigned int hash_val;

        fseek(fp, 0, SEEK_SET);
        f_len = ftell(fp);

        char buffer[MAX_BUFFER_SIZE];
        char key[MAX_KEY_VALUE_SIZE];
        char value[MAX_KEY_VALUE_SIZE];
        len = fread(buffer, sizeof(char), f_len, fp);
        rewind(fp); 
        
        for (i = 0; i < MAX_BUFFER_SIZE && buffer[i] != '\0'; i++) {
                k = 0;
                for (j = i; k < MAX_KEY_VALUE_SIZE - 1 && buffer[j] != ' '; j++) 
                        key[k++] = buffer[j];
                
                j++;
                key[k] = '\0';
                for (k = 0; k < MAX_KEY_VALUE_SIZE - 1 && buffer[j] != ' ' && buffer[j] != '\n'; j++) 
                        value[k++] = buffer[j];
                value[k] = '\0';
                node* newnode = create_node(key, value);
                if (!newnode) {
                        printf("failed to create entry from file\n");
                        return NULL;
                }
                
                hash_val = hash(key);
                insert_node(&table->buckets[hash_val], newnode);
                i = ++j;
        }
        
        fclose(fp);
        return table;
}

bool ht_save(h_table* table, const char* ht_filename) {
        FILE* fp = fopen(ht_filename, "w");
        if (!fp) {
                printf("failed to open file \"%s\"\n", ht_filename);
                return false;
        }
        
        int i, k;
        size_t bytes_written, buffer_size;
        node* n_node;
        char buffer[MAX_BUFFER_SIZE];

        for (i = 0, buffer_size = 0; i < table->size; i++) {
                n_node = table->buckets[i];
                if (n_node == NULL) continue;
                
                for (; n_node->key[buffer_size] != '\0'; buffer_size++) buffer[buffer_size] = n_node->key[buffer_size];
                buffer[++buffer_size] = ' ';

                for (; n_node->value[buffer_size] != '\0'; buffer_size++) buffer[buffer_size] = n_node->value[buffer_size];
                buffer[++buffer_size] = '\n';
        }

        buffer[++buffer_size] = '\0'; 
        bytes_written = fwrite(buffer, sizeof(char), buffer_size, fp);
        if (bytes_written != buffer_size) {
                printf("failed to save data to file\n");
                fclose(fp);
                return false;
        }

        printf("successfully saved data to file\n");
        fclose(fp);

        return true;
}

void ht_free(h_table* table) {
        int i, j;
        node* node; 

        for (i = 0; i < table->size; i++) {
                if (table->buckets[i] == NULL) continue;
        
                node = table->buckets[i];
                for (j = 0; node && node->next; j++) {
                        delete_node(&table->buckets[i], node->key);
                        free(node->key);
                        free(node->value);
                        free(node);
                }
        }

        free(table);
}
