/*
 * NAME, etc.
 *
 * sync.c
 *
 * Synchronization routines for SThread
 */

#define _REENTRANT

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <sched.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include <stdint.h>
#include "sthread.h"
#include "sync.h"

#define MAX_READERS 10  // Max number of waiting readers
/*
 * Atomic operations for x86 architecture.
 */
static inline int test_and_set_bit(volatile unsigned long *addr)
{
	int oldval;
	__asm__ __volatile__("xchgl %0, %1"
			: "=r"(oldval), "+m"(*(addr))	/* output */
			: "0"(1)						/* input */
			: "memory"	/* clobbered: changing contents of memory */
			);
	return oldval;
}

static inline void clear_bit(volatile unsigned long *addr)
{
	// unsigned long oldval;
	// __asm__ __volatile__("xchgl %0, %1"
	// 		: "=r"(oldval), "+m"(*(addr))	/* output */
	// 		: "0"(0)						/* input */
	// 		: "memory"	/* clobbered: changing contents of memory */
	// 		);
	__asm__ __volatile__(
        "lock btr %1, %0"
        : "+m" (*addr)
        : "Ir" (0)
        : "memory"
    );
}

/*
 * rwlock routines
 */

int sthread_rwlock_init(sthread_rwlock_t *rwlock) {
    if (rwlock == NULL) return -1;
    rwlock->readers = 0;
    rwlock->writer = 0;
    rwlock->read_waiters = 0;
    rwlock->write_waiters = 0;
    rwlock->waiting_writer = NO_THREAD;
    rwlock->read_wait_index = 0;
    rwlock->read_wake_index = 0;
    rwlock->lock = 0;
    return 0;
}

int sthread_rwlock_destroy(sthread_rwlock_t *rwlock)
{
        /* FILL ME IN! */
    return 0;
}

int sthread_read_lock(sthread_rwlock_t *rwlock) {
    if (rwlock == NULL) return -1;
    while (1) {
        if (test_and_set_bit((uintptr_t *)&rwlock->lock) == 0) {
            if (rwlock->writer == 0 && rwlock->write_waiters == 0) {
                rwlock->readers++;
                clear_bit((uintptr_t *)&rwlock->lock);
                return 0;
            } else {
                if (rwlock->read_wait_index < MAX_READERS) {
                    rwlock->waiting_readers[rwlock->read_wait_index++] = sthread_self();
                    rwlock->read_waiters++;
                } else {
                    clear_bit((uintptr_t *)&rwlock->lock);
                    return -1;  // Error: too many waiting readers
                }
                clear_bit((uintptr_t *)&rwlock->lock);
                sthread_suspend();
            }
        }
        sched_yield();
    }
    return 0;
}

int sthread_read_try_lock(sthread_rwlock_t *rwlock)
{
        /* FILL ME IN! */
    
}

int sthread_read_unlock(sthread_rwlock_t *rwlock) {
    if (rwlock == NULL) return -1;
    while (test_and_set_bit((uintptr_t *)&rwlock->lock) != 0)
        sched_yield();

    rwlock->readers--;
    if (rwlock->readers == 0 && rwlock->write_waiters > 0 && rwlock->waiting_writer != NO_THREAD) {
        sthread_wake(rwlock->waiting_writer);
        rwlock->waiting_writer = NO_THREAD;
        rwlock->write_waiters--;
    }
    clear_bit((uintptr_t *)&rwlock->lock);
    return 0;
}

int sthread_write_lock(sthread_rwlock_t *rwlock) {
    if (rwlock == NULL) return -1;
    while (1) {
        if (test_and_set_bit((uintptr_t *)&rwlock->lock) == 0) {
            if (rwlock->readers == 0 && rwlock->writer == 0) {
                rwlock->writer = 1;
                clear_bit((uintptr_t *)&rwlock->lock);
                return 0;
            } else {
                if (rwlock->waiting_writer == NO_THREAD) {
                    rwlock->waiting_writer = sthread_self();
                    rwlock->write_waiters++;
                }
                clear_bit((uintptr_t *)&rwlock->lock);
                sthread_suspend();
            }
        }
        sched_yield();
    }
    return 0;
}

int sthread_write_try_lock(sthread_rwlock_t *rwlock)
{
        /* FILL ME IN! */
    
}

int sthread_write_unlock(sthread_rwlock_t *rwlock)
{
        /* FILL ME IN! */
    if (rwlock == NULL) return -1;

    while (test_and_set_bit((uintptr_t *)&rwlock->lock) != 0)
        sched_yield();

    rwlock->writer = 0;
    if (rwlock->write_waiters > 0 && rwlock->waiting_writer != NO_THREAD) {
        sthread_wake(rwlock->waiting_writer);
        rwlock->waiting_writer = NO_THREAD;
        rwlock->write_waiters--;
    } else if (rwlock->read_waiters > 0) {
        while (rwlock->read_wake_index < rwlock->read_wait_index) {
            sthread_wake(rwlock->waiting_readers[rwlock->read_wake_index++]);
            rwlock->read_waiters--;
        }
        rwlock->read_wake_index = 0;
        rwlock->read_wait_index = 0;
    }
    clear_bit((uintptr_t *)&rwlock->lock);
    return 0;
}
