
/* tail: print the last n lines of the input,
 * by default n is 10
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <string.h>

#define _BUF_SIZE 2056
#define DEFAULT_LINES 10

ssize_t get_line(char **restrict lineptr, size_t *restrict n);

int
main(int argc, char **argv)
{
        int nlines = DEFAULT_LINES;
        if (argc > 2)
        {
                printf("Usage: ./tail -n\n");
                return EXIT_FAILURE;
        }
        if (argc == 2)
                nlines = atoi(argv[1]);
	
	if (nlines < 0)
		return EXIT_FAILURE;
        
	char **lineptr = malloc(sizeof(char*) * nlines);
	if (!lineptr)
		return EXIT_FAILURE;

        char *line = NULL;
	int i = 0;
	size_t n = 0, len;

	while ((len = get_line(&line, &n)) >= 0)
	{
		if (i >= nlines)
			i = 0;
		lineptr[i] = malloc(sizeof(char) * len + 1);
		if (!lineptr[i])
			return EXIT_FAILURE;
		strcpy(lineptr[i], line);
	};
	
	i = 0;	
	while (i < nlines)
		printf("%s", lineptr[i]);
        return 0;
}

ssize_t get_line(char **restrict lineptr, size_t *restrict n)
{
        if (lineptr == NULL)
                return -1;

        if (*lineptr == NULL)
        {
                *lineptr = malloc(_BUF_SIZE);
                if (*lineptr == NULL)
                {
                        if (n) *n = 0U;
                        return -1;
                }

                if (n) *n = _BUF_SIZE;
        }

        int c;
        ssize_t len = 0;

        while (len < (ssize_t)(*n - 1) && (c = getchar()) != EOF) 
        {
                (*lineptr)[len++] = c;

		if (c == '\n')
			break;
        };
	
	if (len == 0 && c == EOF)
		return -1;

        (*lineptr)[len] = '\0';
        return len;
}
