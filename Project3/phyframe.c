#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>

/*
#define PAGE_SIZE 128
#define NUM_FRAMES 8

typedef struct {
    int frame_number;
    int last_used;
    bool is_free;
} Frame;
*/
#include "phyframe.h"
Frame frames[NUM_FRAMES];

void init_frames() {
    for (int i = 0; i < NUM_FRAMES; i++) {
        frames[i].frame_number = i;
        frames[i].last_used = -1;
        frames[i].is_free = (i == 0) ? false : true; // Frame 0 reserved for the OS
    }
}

int allocate_frame(int current_time) {
    for (int i = 1; i < NUM_FRAMES; i++) {
        if (frames[i].is_free) {
            frames[i].is_free = false;
            frames[i].last_used = current_time;
            return frames[i].frame_number;
        }
    }
    return -1;
}

int evict_frame(int current_time) {
    int lru_frame = -1;
    int min_time = INT_MAX;
    for (int i = 1; i < NUM_FRAMES; i++) {
        if (frames[i].last_used < min_time) {
            min_time = frames[i].last_used;
            lru_frame = frames[i].frame_number;
        }
    }
    frames[lru_frame].is_free = false;
    frames[lru_frame].last_used = current_time;
    return lru_frame;
}

void update_frame_usage(int frame_number, int current_time) {
    frames[frame_number].last_used = current_time;
}
