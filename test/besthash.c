#include <hashtable.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/time.h>

#define MAX_MALLOC_TEST 51

double wtime(void)
{
    struct timeval t;
    gettimeofday(&t, NULL);
    return (double)t.tv_sec + (double)t.tv_usec * 1E-6;
}

unsigned int jen(const char* s)
{
    unsigned int hash = 0, i;
    for (i = 0; s[i] != '\0'; i++) {
        hash += s[i];
        hash += (hash << 10);
        hash ^= (hash >> 6);
    }
    hash += (hash << 3);
    hash ^= (hash >> 11);
    hash += (hash << 15);
    return hash % HASHTABLE_HEIGHT;
}

unsigned int djb2(const char* s)
{
    unsigned long hash = 5381;
    int c;
    while ((c = *s++))
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

    return hash % HASHTABLE_HEIGHT;
}

unsigned int fnvHash(const char* str)
{
    const unsigned int fnv_prime = 0x811C9DC5;
    unsigned int hash = 0;
    unsigned int i = 0;
    unsigned int len = strlen(str);

    for (i = 0; i < len; i++) {
        hash *= fnv_prime;
        hash ^= (str[i]);
    }

    return hash % HASHTABLE_HEIGHT;
}

unsigned int krHash(const char* s)
{
    unsigned int h = 0, hash_mul = 31;
    while (*s) {
        h = h * hash_mul + (unsigned int)*s;
        s++;
    }
    return h % HASHTABLE_HEIGHT;
}

unsigned int elfHash(const char* s)
{
    unsigned int h = 0, g;
    while (*s) {
        h = (h << 4) + (unsigned int)*s;
        g = h & 0xF0000000L;
        if (g)
            h ^= g >> 24;
        h &= ~g;
        s++;
    }
    return h % HASHTABLE_HEIGHT;
}

void xTestHashTime(unsigned int (*ptr)(const char*), const char* dictionary, const char* filepath, unsigned start_count, unsigned end_count, unsigned step_count)
{
    if (!hashtable_sethashfunc(ptr)) {
        fprintf(stderr, "Error hashtable_sethashfunc\n");
        exit(EXIT_FAILURE);
    }
    FILE* f = fopen(dictionary, "r");
    if (!f) {
        fprintf(stderr, "Error open file: %s\n", dictionary);
        exit(EXIT_FAILURE);
    }
    char** arrStrs = (char**)calloc(end_count, sizeof(*arrStrs));
    if (!arrStrs) {
        fprintf(stderr, "Error malloc arrStrs\n");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < end_count; ++i) {
        arrStrs[i] = (char*)calloc(MAX_MALLOC_TEST, sizeof(**arrStrs));
        if (!arrStrs[i]) {
            fprintf(stderr, "Error malloc arrStrs[%d]\n", i);
            exit(EXIT_FAILURE);
        }
        fgets(arrStrs[i], 50, f);
    }
    fclose(f);
    FILE* out = fopen(filepath, "w");
    if (!out) {
        fprintf(stderr, "Error open file: %s\n", filepath);
        exit(EXIT_FAILURE);
    }
    for (; start_count <= end_count; start_count += step_count) {
        hashtable h = hashtable_initialize();

        double t = wtime();
        for (unsigned j = 0; j < start_count; ++j) {
            hashtable_add(h, arrStrs[j], 1);
        }
        t = wtime() - t;

        hashtable_free(h);

        fprintf(out, "%d\t%lf\n", start_count, t);
    }
    fclose(out);
    for (int i = 0; i < end_count; ++i) {
        free(arrStrs[i]);
    }
    free(arrStrs);
}

static unsigned int (*hfuncptr)(const char*) = NULL;
static unsigned long cols = 0;

bool xtest_hashtable_add(hashtable head, char* key, int value)
{
    if (head && key) {
        unsigned int h = hfuncptr(key);
        if (head[h]) {
            ++cols;
        }
        head[h] = list_addfront(head[h], key, value);
        return true;
    }
    return false;
}

void xTestHashCol(unsigned int (*ptr)(const char*), const char* dictionary, const char* filepath, unsigned start_count, unsigned end_count, unsigned step_count)
{
    if (!ptr) {
        fprintf(stderr, "Error hashtable_sethashfunc\n");
        exit(EXIT_FAILURE);
    }
    hfuncptr = *ptr;
    FILE* f = fopen(dictionary, "r");
    if (!f) {
        fprintf(stderr, "Error open file: %s\n", dictionary);
        exit(EXIT_FAILURE);
    }
    char** arrStrs = (char**)calloc(end_count, sizeof(*arrStrs));
    if (!arrStrs) {
        fprintf(stderr, "Error malloc arrStrs\n");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < end_count; ++i) {
        arrStrs[i] = (char*)calloc(MAX_MALLOC_TEST, sizeof(**arrStrs));
        if (!arrStrs[i]) {
            fprintf(stderr, "Error malloc arrStrs[%d]\n", i);
            exit(EXIT_FAILURE);
        }
        fgets(arrStrs[i], 50, f);
    }
    fclose(f);
    FILE* out = fopen(filepath, "w");
    if (!out) {
        fprintf(stderr, "Error open file: %s\n", filepath);
        exit(EXIT_FAILURE);
    }
    for (; start_count <= end_count; start_count += step_count) {
        hashtable h = hashtable_initialize();

        for (unsigned j = 0; j < start_count; ++j) {
            xtest_hashtable_add(h, arrStrs[j], 1);
        }
        hashtable_free(h);

        fprintf(out, "%d\t%ld\n", start_count, cols);
        cols = 0;
    }
    fclose(out);
    for (int i = 0; i < end_count; ++i) {
        free(arrStrs[i]);
    }
    free(arrStrs);
}

int main(void)
{
    // mkdir("result", S_IRWXU | S_IRWXG | S_IRWXO);
    unsigned start = 200000, end = 1000000, step = start;
    xTestHashTime(jen, "../data/dict1.txt", "result/time/jen.dat", start, end, step);
    xTestHashTime(djb2, "../data/dict1.txt", "result/time/djb2.dat", start, end, step);
    xTestHashTime(fnvHash, "../data/dict1.txt", "result/time/fnvHash.dat", start, end, step);
    xTestHashTime(krHash, "../data/dict1.txt", "result/time/krHash.dat", start, end, step);
    xTestHashTime(elfHash, "../data/dict1.txt", "result/time/elfHash.dat", start, end, step);

    xTestHashCol(jen, "../data/dict1.txt", "result/col/jen.dat", start, end, step);
    xTestHashCol(djb2, "../data/dict1.txt", "result/col/djb2.dat", start, end, step);
    xTestHashCol(fnvHash, "../data/dict1.txt", "result/col/fnvHash.dat", start, end, step);
    xTestHashCol(krHash, "../data/dict1.txt", "result/col/krHash.dat", start, end, step);
    xTestHashCol(elfHash, "../data/dict1.txt", "result/col/elfHash.dat", start, end, step);
}
