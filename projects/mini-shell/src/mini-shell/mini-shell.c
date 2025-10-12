#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#include "mini-shell.h"

int copy(const char *filename_dest, const char *filename_src) {
        FILE *fp = fopen(filename_src, "r");
        FILE *fdest = fopen(filename_dest, "w");
        size_t f_len, buf_len;
        
        if (!fp) {
                printf("Failed to open file \"%s\"\n", filename_src);
                return -1;
        } else if (!fdest) {
                printf("Failed to open file \"%s\"\n", filename_dest);
                fclose(fp);
                return -1;
        }

        if (fseek(fp, 0, SEEK_END) != 0 || (f_len = ftell(fp)) < 0) {
                fclose(fp);
                return -1;
        }

        rewind(fp);
        
        if (f_len > STATIC_BUF_SIZE - 1) {
                printf("error: failed to copy bytes, file size too big '%zu bytes'\n", f_len);
                return -1;
        }

        char destbuf[f_len];
        buf_len = fread(destbuf, sizeof(char), f_len, fp);
        destbuf[buf_len] = '\0';

        fwrite(destbuf, sizeof(char), buf_len, fdest);

        fclose(fp);
        fclose(fdest);

        printf("Successfully copied %zu bytes from \"%s\" to \"%s\"\n", buf_len, filename_src, filename_dest);
        return 0;
}

int del_file(const char *filename) {
        struct stat st;

        if (stat(filename, &st) == 0 && S_ISREG(st.st_mode)) {
                remove(filename);
                printf("Successully removed '%s'\n", filename);
                return 0;
        } else {
                printf("Failed to remove '%s'\n", filename);
                return -1;
        }
}

char *readf(const char *filename) {
        FILE *fp = fopen(filename, "r");
        char *buf;
        size_t f_len, buf_len;

        if (!fp) {
                printf("Failed to open file \"%s\"\n", filename);
                return NULL;
        }
        
        if (fseek(fp, 0, SEEK_END) != 0 || (f_len = ftell(fp)) < 0) {
                fclose(fp);
                return NULL;
        }
        rewind(fp);

        if (f_len > HEAP_BUF_SIZE - 1) {
                fclose(fp);
                return NULL;
        } else {
                buf = malloc(f_len + 1);
                if (!buf) {
                        fclose(fp);
                        return NULL;
                }

                buf_len = fread(buf, sizeof(char), f_len, fp);
                if (buf_len != f_len) {
                        free(buf);
                        fclose(fp);
                        return NULL;
                }

                buf[buf_len] = '\0';

                fclose(fp);
                return buf;
        }
}

int writef(const char *filename, const char *srcbuf) {
        FILE *fp = fopen(filename, "w");
        if (!fp) {
                printf("Failed to open file '%s'\n", filename);
                return -1;
        }
        
        if (fputs(srcbuf, fp) == EOF) {
                printf("Failed to write to file\n");
                fclose(fp);
                return -1;
        }

        printf("successfully written buffer '%s'\n", filename);
        return 0;
}

void appendf(const char *filename, const char *srcbuf) {
        FILE *fp = fopen(filename, "a");
        if (!fp) {
                printf("Failed to open file '%s'\n", filename);
                return;
        }

        fprintf(fp, "%s", srcbuf);
        fclose(fp);

        printf("successfully appended '%s' to file '%s'\n", srcbuf, filename);
}

void help(void) {
        printf("./mini-shell\n\t\tcopy <dest> <src>\n\t\tdelete <filename>\n\t\tread <filename>\n\t\twrite <filename> \"some data\"\n\t\tappend <filename> \"some data\"\n"); 
}
