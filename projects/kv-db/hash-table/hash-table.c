#include <stdio.h> 
#include "hash-table.h"
#include "../node_h/node.h"
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
        return (c <= 'Z' && c >= 'A') || (c >= 'a' && c <= 'z') || (c >= '0' && c <= '9') || (c == ' ');
}

bool ht_create_entry(h_table* table, char *key, char *value) {
        if (!table) {
                printf("hash table is NULL\nexiting now...\n"); 
                exit(1);
        }
        
        if (ht_lfactor(table) >= 0.75f) {
                ht_rehash(&table);
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
        node* temp;
        h_table* new_table = create_table((*table_ref)->size);
        if (!new_table) {
                printf("failed to rehash table\n");
                return;
        }

        for (i = 0; i < (*table_ref)->size; i++) {
                n_head = (*table_ref)->buckets[i];
        
                while (n_head) {
                        temp = n_head;

                        h_v = hash(n_head->key);
                        succ = ht_create_entry(new_table, n_head->key, n_head->value);
        
                        n_head = n_head->next;
                        free(temp->key);
                        free(temp->value);
                        free(temp);

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

	else {
		printf("successfully loaded table from file \"%s\"\n", ht_filename);
	}
        
        char buffer[MAX_BUFFER_SIZE];
        char key[MAX_KEY_VALUE_SIZE];
        char value[MAX_KEY_VALUE_SIZE];
        int i, j, k;
        size_t len, f_len;
        unsigned int hash_val;

        fseek(fp, 0, SEEK_END);
        f_len = ftell(fp);
        rewind(fp);

        len = fread(buffer, sizeof(char), f_len, fp);
        buffer[len] = '\0'; 
        
        for (i = j = k = 0; i < len && buffer[i] != '\0'; i++) {
                while (buffer[i] != ' ' && j < MAX_KEY_VALUE_SIZE - 1) {
                        key[j] = buffer[i];
                        j++;
                        i++;
                }
                key[j] = '\0';
                j = 0;

                while (buffer[i] != '\n' && j < MAX_KEY_VALUE_SIZE - 1) {
                        value[j] = buffer[i];
                        j++;
                        i++;
                }
                value[j] = '\0';
                j = 0;

                ht_create_entry(table, key, value);
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
        
        int i;
        size_t bytes_written, buffer_size;
        node* n_node;
        char buffer[MAX_BUFFER_SIZE];
        size_t key_len, val_len;
        
        for (i = 0, buffer_size = 0; i < table->size; i++) {
                n_node = table->buckets[i];
        
                for (; n_node != NULL; n_node = n_node->next) {
                        key_len = strlen(n_node->key);
                        memcpy(buffer + buffer_size, n_node->key, key_len);
                        buffer_size += key_len;
                        buffer[buffer_size++] = ' ';
        
                        val_len = strlen(n_node->value);
                        memcpy(buffer + buffer_size, n_node->value, val_len);
                        buffer_size += val_len;
                        buffer[buffer_size++] = '\n';
                }
        }

        buffer[buffer_size] = '\0'; 
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
        int i;
        node* curr; 
        node* temp;

        for (i = 0; i < table->size; i++) {
                if (table->buckets[i] == NULL) continue;
        
                curr = table->buckets[i];
                while (curr) {
                        temp = curr;
                        curr = curr->next;

                        free(temp->key);
                        free(temp->value);
                        free(temp);
                }
        }

        free(table);
}
