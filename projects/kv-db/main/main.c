#include <stdio.h> 
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../node_h/node.h"
#include "../hash-table/hash-table.h"

h_table* handle_init();
void handle_command(h_table* table);
void handle_entry_creation(char cmd[], h_table* table);
void handle_entry_deletion(char cmd[], h_table* table);
void handle_entry_get(char cmd[], h_table* table);
bool handle_quit(char cmd[]);
void handle_save(char cmd[], h_table* table);

int main(void) {
        h_table* table = handle_init();         
        bool should_quit;
        
        if (!table) {
                printf("failed to initialize table, quitting now...\n");
                exit(1);
        }
        
        handle_command(table);
        return 0;
}

h_table* handle_init() {
        char buffer[4];
        char filename[32];
        int c, i;
        h_table* table;

        printf("=====> Welcome to Kitty-db! <=====\n\n");
        printf("Do you want to create a new file? (yes/no)\n");
        
        i = 0;
        while ((c = getchar()) != EOF && c != '\n' && i < 3) buffer[i++] = c;
        buffer[i] = '\0';

        if (strcmp(buffer, "yes") == 0) {
                table = create_table(MAX_TABLE_SIZE);
        }

        else {
                printf("Enter file name:\n");
                scanf("%31s", filename);

                table = ht_open(filename);
                if (table) {
                        printf("successfully opened file \"%s\"\n", filename);
                        printf("type \"help\" if you want to show commands\n");
                } 

                else return NULL;
        }

        return table; 
}

void handle_entry_creation(char cmd[], h_table* table) {
        if (!table) {
                printf("table is NULL, exiting now...\n");
                exit(1);
        }

        char operation[12];
        char entry[MAX_KEY_VALUE_SIZE];
        char value[MAX_KEY_VALUE_SIZE];
        int i;
        bool success;

        for (i = 0; cmd[i] != '\0' && cmd[i] != ' ' && i < 7; i++) 
                operation[i] = cmd[i];

        operation[++i] = '\0';        
        
        printf("operation: %s\n", operation);
        if (strcmp(operation, "create") != 0) return;

        for (i++; cmd[i] != ' ' && cmd[i] != '\0' && i < MAX_KEY_VALUE_SIZE - 1; i++) 
                entry[i] = cmd[i];

        entry[++i] = '\0';

        for (i++; cmd[i] != '\0' && cmd[i] != ' ' && i < MAX_KEY_VALUE_SIZE - 1; i++) 
                value[i] = cmd[i];

        value[++i] = '\0';
        
        success = ht_create_entry(table, entry, value);
        if (success) printf("successfully created entry!\n");
}

void handle_entry_deletion(char cmd[], h_table* table) {
        if (!table) {
                printf("table is NULL, exiting now...\n");
                exit(1);
        }

        char operation[12];
        char key[MAX_KEY_VALUE_SIZE];
        int i; 

        for (i = 0; i < 11 && cmd[i] != ' '; i++) 
                operation[i] = cmd[i];

        operation[++i] = '\0';
        if (strcmp(operation, "remove") != 0) return;
        
        for (i++; i < MAX_KEY_VALUE_SIZE - 1; i++)
                key[i] = cmd[i];
        key[++i] = '\0';

        ht_delete_entry(table, key);
}

void handle_entry_get(char cmd[], h_table* table) {
        if (!table) {
                printf("table is NULL, exiting now...\n");
                exit(1);
        }

        char operation[12];
        char key[MAX_KEY_VALUE_SIZE];
        int i; 

        for (i = 0; i < 11 && cmd[i] != ' '; i++) 
                operation[i] = cmd[i];

        operation[++i] = '\0';
        if (strcmp(operation, "get") != 0) return;
        
        for (i++; i < MAX_KEY_VALUE_SIZE - 1; i++)
                key[i] = cmd[i];
        key[++i] = '\0';

        char* val = ht_get_entry(table, key);
        if (val) printf("Value for key \"%s\" found: %s\n", key, val);
}

void handle_save(char cmd[], h_table* table) {
        if (!table) {
                printf("table is NULL, exiting now...\n");
                exit(1);
        }

        char operation[12];
        char filename[16];
        int i; 

        for (i = 0; i < 11 && cmd[i] != ' '; i++) 
                operation[i] = cmd[i];

        operation[++i] = '\0';
        if (strcmp(operation, "save") != 0) return;
        for (i++; i < 15; i++)
                filename[i] = cmd[i];
        filename[++i] = '\0';


        ht_save(table, filename); 
}

bool handle_quit(char cmd[]) {
        if (strcmp(cmd, "quit") == 0) {
                return true;
        }

        else return false;
}

void handle_command(h_table* table) {
        if (!table) {
                printf("table is NULL, exiting now...\n");
                exit(1);
        }
        
        while (1) {
                char cmd[32];

                printf("kitty-db> ");
                scanf("%31s", cmd); 
               
                handle_entry_creation(cmd, table);
                handle_entry_get(cmd, table);
                handle_entry_deletion(cmd, table);
                handle_save(cmd, table);

                if (handle_quit(cmd)) {
                        ht_save(table, "kitty-db.txt");
                        ht_free(table);
                        break;
                }
        }        
}
