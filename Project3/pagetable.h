#ifndef PAGETABLE_H
#define PAGETABLE_H

#define PAGE_ENTRIES 256

typedef struct {
    int frame_number;
    int valid;
} PageTableEntry;

typedef struct {
    int virtual_page_number;
    int frame_number;
} ReverseMapping;

// Function declarations
void init_page_table();
void init_reverse_mapping();
void update_reverse_mapping(int frame_number, int virtual_page_number);
int translate_address(unsigned long virtual_address, int current_time);

#endif // PAGETABLE_H
