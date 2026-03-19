#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <syscalls.h> // This is in my machine's /usr/local/include, 
                      // you can also find it in /lib/

int main(int argc, char **argv) {
    int mode = (argc > 1) ? 1 : 0;
    int nfiles = 1;
    
    int mgetchar(int fd);
    void error(char *fmt, ...);
    while (!mode || nfiles < argc) {
        ssize_t n;
        char buf[BUFSIZ];
        int fd;

        if ((fd = (mode) ? open(argv[nfiles], O_RDONLY, 0) : 0) == -1)
            error("failed to open file %s", argv[nfiles]);

        while ((n = read(fd, buf, sizeof buf)) > 0)
            if (write(1, buf, n) != n)
                error("cat: write error");
        if (!mode) break;
        nfiles++;
    }

    exit(EXIT_SUCCESS);
}

int mgetchar(int fd) {
    static char buf[BUFSIZ];
    static char *bufp;
    static int n;
    
    if (n == 0) {
        n = read(fd, buf, sizeof buf);
        bufp = buf;
    }

    return (--n >= 0) ? (unsigned char) *bufp++ : EOF;
}

void error(char *fmt, ...) {
    va_list args;

    va_start(args, fmt);
    fprintf(stderr, "error: ");
    vprintf(fmt, args);
    fprintf(stderr, "\n");
    va_end(args);

    exit(EXIT_FAILURE);
}
