#include "include/io.h"
#include <stdio.h>
#include <stdlib.h>

char *read_file(const char *filename)
{
    FILE *file = fopen(filename, "r");

    if (!file)
    {
        fprintf(stderr, "Error opening file: %s\n", filename);
        exit(1);
    }

    fseek(file, 0, SEEK_END);
    long length = ftell(file);
    fseek(file, 0, SEEK_SET);

    char *buffer = calloc(1, length + 1);
    fread(buffer, 1, length, file);
    fclose(file);

    return buffer;
}