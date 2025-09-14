#include <stdio.h> 
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../node_h/node.h"
#include "../hash-table/hash-table.h"

h_table* handle_init(char filename[]);
void handle_command();
void handle_debug(h_table* table);
void handle_entry_creation(h_table* table);
void handle_entry_deletion(h_table* table);
void handle_entry_get(h_table* table);
void handle_help();
void handle_exit(h_table* table, const char* db_filename);

int main(void) {
	handle_command();

        return 0;
}

h_table* handle_init(char filename[]) {
	char op[6];
        const char* db_name = "kitty-db.txt";
	h_table* table;

	printf("===> Welcome to kitty-db! <===\n\n");
	printf("To seek help, type \"help\"\n");
	
	printf("Do you want to create a new db? (yes/no) ");
	
	scanf("%5s", op);
	if (strcmp(op, "yes") == 0) {
		table = create_table(MAX_TABLE_SIZE);
                memcpy(filename, db_name, strlen(db_name));
	}

	else {
		printf("\nEnter file name: ");
		scanf("%31s", filename);
		table = ht_open(filename);
	}
	
	if (!table) {
		printf("table is NULL, exiting now...\n");
		exit(1);
	}


	return table;
}

void handle_entry_deletion(h_table* table) {
	char key[MAX_KEY_VALUE_SIZE];
	
	printf("Enter key: ");
	scanf("%19s", key);
	
	ht_delete_entry(table, key);
}

void handle_debug(h_table* table) {
	if (table->num_of_elements == 0) {
		printf("table is empty!\n");
		return;
	}

	int i;
	char key[MAX_KEY_VALUE_SIZE];
	char value[MAX_KEY_VALUE_SIZE];
	node* head;

	for (i = 0; i < table->size; i++) {
		head = table->buckets[i];
		if (head == NULL) continue;
	
		while (head != NULL) {
			printf("[%s:%s]->", head->key, head->value);
			head = head->next;
		}
		
		printf("NULL\n");
	}

        printf("number of elements: %d\n", table->num_of_elements);
}

void handle_entry_creation(h_table* table) {
	if (!table) {
		printf("table is NULL, exiting now...\n");
		exit(1);
	}

	char key[MAX_KEY_VALUE_SIZE];
	char value[MAX_KEY_VALUE_SIZE];
	bool succ;

	printf("Enter key: ");
	scanf("%19s", key);
	printf("Enter value: ");
	scanf("%19s", value);

	succ = ht_create_entry(table, key, value);
	if (succ) {
		printf("successfully created key-value pair \"%s\" : \"%s\"\n", key, value);
	}
}

void handle_exit(h_table* table, const char* db_filename) {
        ht_save(table, db_filename);
        ht_free(table);
}

void handle_entry_get(h_table* table) {
        char* value;
        char key[32];
        unsigned long hash_val;
        
        printf("Enter key: ");
        scanf("%31s", key);

        value = ht_get_entry(table, key);

        if (value) printf("found value: \"%s\"\n", value);
}

void handle_help() {
        printf("Type:\n");
        printf("\tcreate\t\tto create a key value pair\n");
        printf("\tdelete\t\tto delete a pair\n");
        printf("\t   get\t\tto get a value from a key\n");
        printf("\t  exit\t\tto save and exit program\n");
        printf("\t debug\t\tto debug and show table\n");
}

void handle_command() {
	h_table* table; 
	char cmd[32];
        char filename[32];
	
	table = handle_init(filename);
	
	while (true) {
		printf("kitty-db> ");
		
		scanf("%31s", cmd);
		if (strcmp(cmd, "create") == 0) 
			handle_entry_creation(table);

                else if (strcmp(cmd, "debug") == 0) 
			handle_debug(table);

                else if (strcmp(cmd, "delete") == 0) 
			handle_entry_deletion(table);
                
                else if (strcmp(cmd, "get") == 0) {
                        handle_entry_get(table);
                }

                else if (strcmp(cmd, "exit") == 0) {
                        handle_exit(table, filename);
                        break;
                }
                
                else if (strcmp(cmd, "help") == 0) {
                        handle_help();
                }
                
                else {
                        printf("Unknown command: \"%s\"\n", cmd);
                }
	}
}
