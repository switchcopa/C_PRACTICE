#ifndef HASH_TABLE_H 
#define HASH_TABLE_H 

#include "node.h"
#include <stdbool.h>
#include <stdlib.h>

#define MAX_TABLE_SIZE 1000
#define MAX_KEY_VALUE_SIZE 20
typedef struct {
        node* buckets[MAX_TABLE_SIZE];
        size_t size;
        int num_of_elements;
} h_table;

h_table* create_table(size_t size);
unsigned long hash(char* key);
bool is_valid_character(char c);
bool ht_create_entry(h_table* table, char* key, char* value);
bool ht_delete_entry(h_table* table, char* key);
char* ht_get_entry(h_table* table, char* key);
float ht_lfactor(h_table* table);
void ht_rehash(h_table** table_ref);
h_table* ht_open(const char* ht_filename);
bool ht_save(h_table* table, const char* ht_filename);
void ht_free(h_table* table);

#endif 
