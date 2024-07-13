
/*
 * This programm is pretty much identical to the one from the
 * previous example, except, that it operates on a union instead
 */

#include <stdio.h>
#include <stdlib.h>

#include "include/cstunn.h"

#include "simple_union.h"

/*
 * For this example we simply use this static buffer
 * which is known to have enough capacity for the
 * input files
 */
char buffer[1000];

/* Utility */
void
readfilex(
    const char *path);

int main(
    int argc,
    char **argv)
{
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <path>\n", argv[0]);
        return 1;
    }

    readfilex(argv[1]);

    return 0;
}

void
readfilex(
    const char *path)
{
    FILE *fp = fopen(path, "r");
    cstunn_error_t err;
    size_t size;
    char *end;
    union simple_union sun = { 0 };

    if (!fp) {
        fprintf(stderr, "Failed to open \"%s\"\n", path);
        exit(1);
    }
    
    size = fread(buffer, 1, sizeof(buffer) - 1, fp);
    if (size == 0) {
        fprintf(stderr, "Failed to read \"%s\"\n", path);
        exit(1);
    }
    buffer[size] = 0;

    fclose(fp);

    err = cstunn_parse(
        &sun,
        buffer,
        simple_union_spec,
        &end,
        0);
    
    if (err) {
        fprintf(stderr, "%s:", path);
        cstunn_print_error(
            stderr,
            buffer,
            end,
            err);
    }
    else {
        simple_union_print(
            &sun);
    }
}