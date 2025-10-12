#ifndef MINI_SHELL_H
#define MINI_SHELL_H

#define FILENAME_SIZE 32
#define STATIC_BUF_SIZE 8192
#define HEAP_BUF_SIZE 16384
#define MAX_FILES 128

int             copy(const char *filename_dest, const char *filename_src);
int             del_file(const char *filename);
char           *readf(const char *filename);
int             writef(const char *filename, const char *srcbuf);
void            appendf(const char *filename, const char *srcbuf);
void            ftouch(const char *filenames, int nfiles);
void            help(void);

#endif
