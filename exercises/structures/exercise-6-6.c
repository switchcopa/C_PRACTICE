#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <stdbool.h>
#include <ctypes.h>
#include <sys/types.h>

#define HASHSIZE    101
#define BUFSIZE     256
#define FBUFSIZE    1000000
#define WORD_SIZE   32
static struct nlist *hashtab[HASHSIZE];

struct nlist
{
    struct nlist *next;
    char *name;
    char *defn;
};

unsigned hash(const char *s);
struct nlist *lookup(const char *s);
char *strdup(const char *s);
struct nlist *install(const char *name, const char *defn);

bool undef(const char *name);
static void freenlist(struct nlist *p);

// utils
char *loadfile(const char *filename, ssize_t *size);
void appendbuf(char *buf, int *i, int c);

int main(int argc, char **argv)
{
    if (argc != 2) {
        fprintf(stderr, "Usage: ./app \"filename.c\\n");
        return EXIT_FAILURE;
    }

    const char *filename = argv[1];
    ssize_t size = 0;
    char *fbuf = loadfile(filename, &size);
    char *outbuf = malloc(sizeof(char) * (FBUFSIZE + 1));
    int outbufp = 0;
    if (!fbuf || !outbuf || !size)
        goto allocerr;

    enum {
        NORMAL,
        COMMENT,
        MCOMMENT,
        STRING,
        CHAR,
    } state = NORMAL;

    int c, prev = 0;
    for (int i = 0; i < size && (c = fbuf[i]); i++)
    {
        switch (state) {

            case NORMAL:
                if (c == '"') state = STRING;
                else if (c == '\'') state = CHAR;
                else if (prev == '/' && c == '/') state = COMMENT;
                else if (prev == '/' && c == '*') state = MCOMMENT;
                else if (c == '#') {
                    char word[WORD_SIZE];
                    int j = 0;

                    // read preprocessor directive until fbuf[i] is space or something else
                    while (i < size && isspace(fbuf[i++]))
                        ;
                    while (i < size && isalpha(fbuf[i]) && j < WORD_SIZE - 1)
                        word[j++] = fbuf[i++];
                    word[j] = '\0';
                    if (strcmp(word, "#define") == 0) {
                        while (i < size && isspace(fbuf[i++]))
                            ;
                        char name[WORD_SIZE];
                        char value[WORD_SIZE];
                        int k = 0;

                        while (i < size && (isalnum(fbuf[i]) || fbuf[i] == '_') && k < WORD_SIZE - 1)
                            name[k++] = fbuf[i++];
                        name[k] = '\0';

                        while (i < size && isspace(fbuf[i++]))
                            ;
                        k = 0;
                        while (i < size && !isspace(fbuf[i]) && k < WORD_SIZE - 1)
                            value[k++] = fbuf[i++];
                        value[k] = '\0';

                        install(name, value);
                    }
                }
                break;

            case STRING:
                if (c == '"' && prev != '\\') state = NORMAL;
                appendbuf(outbuf, &outbufp, c);
                break;
            case CHAR:
                if (c == '\'' && prev != '\\') state = NORMAL;
                appendbuf(outbuf, &outbufp, c);
                break;
            case COMMENT:
                if (c == '\n') state = NORMAL;
                appendbuf(outbuf, &outbufp, c);
                break;
            case MCOMMENT:
                if (prev == '*' && c == '/') state = NORMAL;
                appendbuf(outbuf, &outbufp, c);
                break;
        }

        prev = c;
    }
    return EXIT_SUCCESS;
allocerr:
    fprintf(stderr, "allocation failure\n");
    return EXIT_FAILURE;
}

unsigned hash(const char *s)
{
    unsigned hashval;

    for (hashval = 0; *s != '\0'; s++)
        hashval = *s + 31 * hashval;
    return hashval % HASHSIZE;
}

struct nlist *lookup(const char *s)
{
    struct nlist *np;

    for (np = hashtab[hash(s)]; np != NULL; np = np->next)
        if (strcmp(s, np->name) == 0)
            return np;
    return NULL;
}

char *strdup(const char *s)
{
    size_t len = strlen(s);
    char *p = malloc(sizeof(char) * (len + 1));
    if (!p) return NULL;

    memcpy(p, s, len);
    p[len] = '\0';
    return p;
}

struct nlist *install(const char *name, const char *defn)
{
    struct nlist *np;
    unsigned hashval;

    if ((np = lookup(name)) == NULL) {
        np = (struct nlist *) malloc(sizeof(*np));
        if (np == NULL || (np->name = strdup(name)) == NULL)
            return NULL;
        hashval = hash(name);
        np->next = hashtab[hashval];
        hashtab[hashval] = np;
    } else
        free((void *)np->defn);
    if ((np->defn = strdup(defn)) == NULL)
        return NULL;
    return np;
}

/* undef: return true if name is in the hash table and delete it, else false*/
bool undef(const char *name)
{
    struct nlist *np;
    struct nlist *prev = NULL;
    unsigned hashval = hash(name);

    for (np = hashtab[hashval]; np != NULL; np = np->next) {
        if (strcmp(name, np->name) == 0) {
            struct nlist *p = hashtab[hashval];
            if (prev == NULL)
                hashtab[hashval] = np->next;
            else
                prev->next = np->next;
            freenlist(p);
            return true;
        }
        prev = np;
    }

    return false;
}

static void freenlist(struct nlist *p) {
    free(p->name);
    free(p->defn);
    free(p);
}

char *loadfile(const char *filename, ssize_t *outsize)
{
    FILE *fp = fopen(filename, "r");
    char *fbuffer = malloc(sizeof(char) * (FBUFSIZE + 1));
    if (!fp || !fbuffer) goto err;

    fseek(fp, 0, SEEK_END);
    size_t fsize = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    
    size_t n = fread(fbuffer, sizeof (char), fsize, fp);
    if (n < fsize && ferror(fp)) goto err;

    fbuffer[size] = '\0';
    fclose(fp);

    *outsize = fsize;
    return fbuffer;
err:
    if (!fp)
        return NULL;

    fclose(fp);
    *outsize = fsize;
    return NULL;
}

void appendbuf(char *buf, int *i, char c)
{
    buf[(*i)++] = c;
}
