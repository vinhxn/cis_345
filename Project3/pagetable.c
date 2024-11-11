#include <stdio.h>
#include <stdlib.h>

/*
#define PAGE_ENTRIES 256

typedef struct {
    int frame_number;
    int valid;
} PageTableEntry;

typedef struct {
    int virtual_page_number;
    int frame_number;
} ReverseMapping;
*/
#include "pagetable.h"
#include "phyframe.h"

PageTableEntry page_table[PAGE_ENTRIES];
ReverseMapping reverse_mapping[NUM_FRAMES];

void init_page_table() {
    for (int i = 0; i < PAGE_ENTRIES; i++) {
        page_table[i].frame_number = -1;
        page_table[i].valid = 0;
    }
}

void init_reverse_mapping() {
    for (int i = 0; i < NUM_FRAMES; i++) {
        reverse_mapping[i].virtual_page_number = -1;
        reverse_mapping[i].frame_number = -1;
    }
}

void update_reverse_mapping(int frame_number, int virtual_page_number) {
    reverse_mapping[frame_number].virtual_page_number = virtual_page_number;
    reverse_mapping[frame_number].frame_number = frame_number;
}

int translate_address(unsigned long virtual_address, int current_time) {
    unsigned int page_number = (virtual_address / PAGE_SIZE);
    unsigned int offset = virtual_address % PAGE_SIZE;

    if (page_number >= PAGE_ENTRIES) {
        fprintf(stderr, "Invalid page number: %u\n", page_number);
        exit(EXIT_FAILURE);
    }

    if (page_table[page_number].valid) {
        update_frame_usage(page_table[page_number].frame_number, current_time);
        return (page_table[page_number].frame_number * PAGE_SIZE) + offset;
    } else {
        int frame_number = allocate_frame(current_time);
        if (frame_number == -1) {
            frame_number = evict_frame(current_time);
            int old_virtual_page = reverse_mapping[frame_number].virtual_page_number;
            page_table[old_virtual_page].valid = 0;
        }
        page_table[page_number].frame_number = frame_number;
        page_table[page_number].valid = 1;
        update_reverse_mapping(frame_number, page_number);
        return (frame_number * PAGE_SIZE) + offset;
    }
}
