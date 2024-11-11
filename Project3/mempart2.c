#include <stdio.h>
#include <stdlib.h>
#include "phyframe.h"
#include "pagetable.h"

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

    FILE *output_file = fopen("part2out", "w");
    if (!output_file) {
        perror("Error opening output file");
        fclose(input_file);
        return EXIT_FAILURE;
    }

    init_frames();
    init_page_table();
    init_reverse_mapping();

    unsigned long virtual_address;
    int current_time = 0;
    int page_faults = 0;

    while (fread(&virtual_address, sizeof(unsigned long), 1, input_file) == 1) {
        unsigned long physical_address = translate_address(virtual_address, current_time);
        if (physical_address == -1) {
            page_faults++;
        }
        printf("0x%016lX -> 0x%04lX\n", virtual_address, physical_address);
        fprintf(output_file, "0x%04lX\n", physical_address);
        current_time++;
    }

    fclose(input_file);
    fclose(output_file);

    // Compute the MD5 checksum
    system("md5sum part2out > p2result.txt");

    // Report page faults
    FILE *result_file = fopen("p2result.txt", "a");
    if (result_file) {
        fprintf(result_file, "Page Faults: %d\n", page_faults);
        fclose(result_file);
    }

    return EXIT_SUCCESS;
}
