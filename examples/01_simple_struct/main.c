
/*
 * This programm takes one file-path as a commandline-argument
 * and parses that file as a simple_struct. On success prints
 * the file, and on failure prints an error
 * 
 * Either enter the path to the example*.cstunn files in the
 * directory or add your own
 */

#include <stdio.h>
#include <stdlib.h>

#include "include/cstunn.h"

#include "simple_struct.h"

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
    struct simple_struct sst = { 0 };

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
        &sst,
        buffer,
        simple_struct_spec,
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
        simple_struct_print(
            &sst);
    }
}