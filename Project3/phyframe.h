#ifndef PHYFRAME_H
#define PHYFRAME_H

#include <stdbool.h>

#define PAGE_SIZE 128
#define NUM_FRAMES 8

typedef struct {
    int frame_number;
    int last_used;
    bool is_free;
} Frame;

// Function declarations
void init_frames();
int allocate_frame(int current_time);
int evict_frame(int current_time);
void update_frame_usage(int frame_number, int current_time);

#endif // PHYFRAME_H
