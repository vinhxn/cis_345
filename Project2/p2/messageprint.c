/*
 * messageprint.c
 *
 */
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

//Initialize Global Variables and Include Necessary Headers
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t CV = PTHREAD_COND_INITIALIZER;
int number_prime_finished = 0;
int total_threads;
bool *is_prime;

//Function to Check Prime Numbers
bool check_prime(int num) {
    if (num <= 1) return false;
    for (int i = 2; i * i <= num; i++) {
        if (num % i == 0) return false;
    }
    return true;
}

//Thread function
void* thread_function(void* arg) {
    int thread_id = *(int*)arg;

    pthread_mutex_lock(&lock);
    if (is_prime[thread_id]) {
        printf("Prime Thread %d: Printing message\n", thread_id);
        number_prime_finished++;
        if (number_prime_finished == total_threads) {
            pthread_cond_broadcast(&CV);
        }
    } else {
        while (number_prime_finished < total_threads) {
            pthread_cond_wait(&CV, &lock);
        }
        printf("Composite Thread %d: Printing message\n", thread_id);
    }
    pthread_mutex_unlock(&lock);
    return NULL;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <number_of_threads>\n", argv[0]);
        return 1;
    }

    total_threads = atoi(argv[1]);
    if (total_threads < 20) {
        fprintf(stderr, "Number of threads should be at least 20.\n");
        return 1;
    }

    pthread_t threads[total_threads];
    int thread_ids[total_threads];
    is_prime = malloc(total_threads * sizeof(bool));

    for (int i = 0; i < total_threads; i++) {
        thread_ids[i] = i;
        is_prime[i] = check_prime(i);
    }

    for (int i = 0; i < total_threads; i++) {
        pthread_create(&threads[i], NULL, thread_function, &thread_ids[i]);
    }

    for (int i = 0; i < total_threads; i++) {
        pthread_join(threads[i], NULL);
    }

    free(is_prime);
    return 0;
}
