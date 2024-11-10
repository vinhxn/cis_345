#include <stdio.h>
#include <stdlib.h>

#define PAGE_SIZE 256
#define PAGE_ENTRIES 7

// Hardcoded page table
unsigned int page_table[PAGE_ENTRIES] = { 0x02, 0x04, 0x01, 0x07, 0x03, 0x05, 0x06};

unsigned long translate_address(unsigned long virtual_address) {
    unsigned int page_number = (virtual_address >> 8) & 0xFF;
    unsigned int offset = virtual_address & 0xFF;

    if (page_number >= PAGE_ENTRIES) {
        fprintf(stderr, "Invalid page number: %u\n", page_number);
        exit(EXIT_FAILURE);
    }

    unsigned int frame_number = page_table[page_number];
    unsigned long physical_address = (frame_number << 8) | offset;

    return physical_address;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <input_sequence_filename>\n", argv[0]);
        return EXIT_FAILURE;
    }

    FILE *input_file = fopen(argv[1], "rb");
    if (!input_file) {
        perror("Error opening input file");
        return EXIT_FAILURE;
    }

    FILE *output_file = fopen("part1out", "w");
    if (!output_file) {
        perror("Error opening output file");
        fclose(input_file);
        return EXIT_FAILURE;
    }

    unsigned long virtual_address;
    while (fread(&virtual_address, sizeof(unsigned long), 1, input_file) == 1) {
        unsigned long physical_address = translate_address(virtual_address);
        //printf("0x%016lx\n -> 0x%04lx\n", virtual_address, physical_address);
        fprintf(output_file, "0x%04lx\n", physical_address);
    }

    fclose(input_file);
    fclose(output_file);

    // Compute the md5 checksum
    system("md5sum part1out > p1result.txt");

    return EXIT_SUCCESS;
}