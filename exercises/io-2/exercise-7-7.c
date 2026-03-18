#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFSIZE 8024

int main(int argc, char **argv) {
    if (argc < 3) {
        fprintf(stderr, "Usage: ./grep \"pattern\" file1 file2 ...\n")
        exit(EXIT_FAILURE);
    }

    const char *pattern = argv[1];
    int nfiles = 2;

    int strstr(const char *dest, const char *pattern);
    int getline(char *dest, int max, FILE *fsrc);

    char line[BUFSIZE];
    while (nfiles < argc) {
        FILE *fp = fopen(argv[nfiles], "r");
        if (!fp) { fprintf(stderr, "failed to open %s\n", argv[nfiles--]; continue }
        while (getline(line, BUFSIZE, fp))
            if (strstr(line, pattern))
                printf("%s: %s", argv[nfiles], line);
        nfiles--;
    }
}

int getline(char *dest, int max, FILE *fsrc) {
    if (fgets(dest, max, fsrc) == NULL)
        return 0;
    else return strlen(dest);
}

int strstr(const char *dest, const char *pattern) {
    int i, j, k;
    for (i = 0; dest[i] != '\n'; i++)
        if (dest[i] == pattern[0]) {
            for (j = 0, k = i; pattern[j] != '\0' && dest[i] == pattern[j]; i++, j++)
                ;
            if (pattern[j] == '\0')
                return 1;
            else
                i = k;
        }

    return 0;
}
