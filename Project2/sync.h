/*
 * NAME, etc.
 *
 * sync.h
 */

#ifndef _STHREAD_SYNC_H_
#define _STHREAD_SYNC_H_

struct sthread_rwlock_struct {
        /* FILL ME IN! */
    int readers;        // Number of active readers holding the lock
    int writer;         // Flag indicating if a writer holds the lock (0 or 1)
    int read_waiters;   // Number of readers waiting for the lock
    int write_waiters;  // Number of writers waiting for the lock
    sthread_t waiting_writer;  // The thread waiting for the write lock
    sthread_t waiting_readers[10]; // Array to store waiting reader threads
    int read_wait_index;  // Index for waiting readers
    int read_wake_index;  // Index for waking readers
    int lock;           // Atomic lock for synchronization
};

typedef struct sthread_rwlock_struct sthread_rwlock_t;

int sthread_rwlock_init(sthread_rwlock_t *rwlock);
int sthread_rwlock_destroy(sthread_rwlock_t *rwlock);
int sthread_read_lock(sthread_rwlock_t *rwlock);
int sthread_read_try_lock(sthread_rwlock_t *rwlock);
int sthread_read_unlock(sthread_rwlock_t *rwlock);
int sthread_write_lock(sthread_rwlock_t *rwlock);
int sthread_write_try_lock(sthread_rwlock_t *rwlock);
int sthread_write_unlock(sthread_rwlock_t *rwlock);

#endif
