#include "mini-shell/mini-shell.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char **argv) {
        if (argc == 1) {
                printf("Usage: ./test help\n");
                return 1;
        }
        
        if (strcmp(argv[1], "copy") == 0 && argc == 4)
                copy(argv[2], argv[3]);

        else if (strcmp(argv[1], "delete") == 0 && argc == 3)
                del_file(argv[2]);

        else if (strcmp(argv[1], "read") == 0 && argc == 3) {
                char *buf = readf(argv[2]);
                if (!buf)
                        return -1;

                printf("%s\n", buf);
                free(buf);
        }
                
        else if (strcmp(argv[1], "write") == 0 && argc == 4)
                writef(argv[2], argv[3]);

        else if (strcmp(argv[1], "append") == 0 && argc == 4)
                appendf(argv[2], argv[3]);
        
        else if (strcmp(argv[1], "help") == 0)
                help();
        
        else
                printf("Unknown arguments or commands\n");

        return 0;
}
