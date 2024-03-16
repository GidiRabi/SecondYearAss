// compress.c
#include "compress.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int is_valid_input_format(const char *filename, const char *expected_extension) {
    size_t filename_len = strlen(filename);
    size_t ext_len = strlen(expected_extension);
    if (filename_len < ext_len + 1) // The filename is shorter than the extension plus the dot
        return 0;

    const char *extension = filename + (filename_len - ext_len);
    if (strcmp(extension, expected_extension) != 0)
        return 0;

    return 1;
}

void compress_file(const char *input_filename) {
    FILE *input_file = fopen(input_filename, "r");
    if (input_file == NULL) {
        fprintf(stderr, "Error opening input file.\n");
        exit(1);
    }

    if(!is_valid_input_format(input_filename, ".txt")) {
        fprintf(stderr, "Invalid input file format.\n");
        fclose(input_file);
        exit(1);
    }

    // Create output file with .bin extension
    char output_filename[strlen(input_filename) + 1]; // Adjusted for new filename
    strcpy(output_filename, input_filename);
    char *last_dot = strrchr(output_filename, '.');
    if (last_dot != NULL && strcmp(last_dot, ".txt") == 0) {
        *last_dot = '\0'; // Remove .txt extension
    }
    strcat(output_filename, ".bin");

    FILE *output_file = fopen(output_filename, "wb");
    if (output_file == NULL) {
        fprintf(stderr, "Error creating output file.\n");
        fclose(input_file);
        exit(1);
    }

    int digit;
    while ((digit = fgetc(input_file)) != EOF) {
        for (int i = 3; i >= 0; i--) {
            int bit = (digit >> i) & 1; // Extract the ith bit from digit
            bit = (bit == 1) ? 0 : 1;
            fprintf(output_file, "%d", bit); // Write the bit to the output file
        }
    }

    // Pad with zeros if necessary
    if (ftell(input_file) % 2 != 0) {
        fprintf(output_file, "0000"); // Write four zeros
    }
    fclose(input_file);
    fclose(output_file);
}

void decompress_file(const char *input_filename) {
    FILE *input_file = fopen(input_filename, "rb");
    if (input_file == NULL) {
        fprintf(stderr, "Error opening input file.\n");
        exit(1);
    }

    if(!is_valid_input_format(input_filename, ".bin")) {
        fprintf(stderr, "Invalid input file format.\n");
        fclose(input_file);
        exit(1);
    }

    // Create output filename without .bin extension
    char output_filename[strlen(input_filename) - 3 + 5]; // Adjusted for ".txt"
    strncpy(output_filename, input_filename, strlen(input_filename) - 4);
    output_filename[strlen(input_filename) - 4] = '\0';
    strcat(output_filename, ".txt");

    FILE *output_file = fopen(output_filename, "w");
    if (output_file == NULL) {
        fprintf(stderr, "Error creating output file.\n");
        fclose(input_file);
        exit(1);
    }


    char bit;
    int number = 0;
    int power = 3;
    while ((bit = fgetc(input_file)) != EOF) {

        // Convert character '0' or '1' to integer 0 or 1
        bit = (bit == '0') ? '1' : '0';
        int bit_value = bit - '0';

        // Set the corresponding bit in the number
        number |= (bit_value << power);

        // Decrement the power
        power--;

        // If all 4 bits are processed
        if (power == -1) {
            // Write the number to output file
            if(number <=9 && number >=0) {
                fprintf(output_file, "%d", number);
            }

            // Reset the variables for next number
            number = 0;
            power = 3;
        }
    }

    fclose(input_file);
    fclose(output_file);
}
