#include "compress.h"
#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Usage: %s -c/-d file\n", argv[0]);
        return 1;
    }

    char *flag = argv[1];
    char *filename = argv[2];

    if (strcmp(flag, "-c") == 0) {
        compress_file(filename);
    } else if (strcmp(flag, "-d") == 0) {
        decompress_file(filename);
    } else {
        printf("Invalid flag: %s\n", flag);
        return 1;
    }

    return 0;
}