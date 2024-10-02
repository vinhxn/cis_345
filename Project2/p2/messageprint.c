/*
 * messageprint.c
 * Nhat Nguyen
 */
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t CV = PTHREAD_COND_INITIALIZER;
int number_prime_finished = 0; // number of prime printed
int total_threads; // total number of threads
int total_prime_threads = 0; //count number of prime threads

// Function to check if the number of prime
bool isPrime(int num) {
    if (num <= 1) return false;
    for (int i = 2; i * i <= num; i++) {
        if (num % i == 0) return false;
    }
    return true;
}

//Thread function
void* thread_function(void* arg) {
    int thread_id = *((int*)arg);

    pthread_mutex_lock(&lock);
    if (!isPrime(thread_id)) {
        //if this thread_id is not prime, wait until all prime threads have printed
        while (number_prime_finished < total_prime_threads) {
            pthread_cond_wait(&CV, &lock);
        }
    }
    pthread_mutex_unlock(&lock);
    //print thread
    printf("Thread %d: Hello, I am %s.\n", thread_id, isPrime(thread_id) ? "prime" : "composite");

    pthread_mutex_lock(&lock);

    //count prime threads
    if (isPrime(thread_id)) {
        number_prime_finished++;
        //if all prime threads are finished, wakes up all threads that are sleeping
        if (number_prime_finished == total_prime_threads) {
            pthread_cond_broadcast(&CV);
        }
    }
    pthread_mutex_unlock(&lock);

    return NULL;
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <number_of_threads>\n", argv[0]);
        return 1;
    }

    total_threads = atoi(argv[1]);
    if (total_threads < 1) {
        fprintf(stderr, "Number of threads must be at least 1.\n");
        return 1;
    }

    // calculate number of prime threads
    for(int i = 1; i <= total_threads; i++) {
        if(isPrime(i))
            total_prime_threads++;
    }

    // create threads
    pthread_t threads[total_threads];
    int thread_ids[total_threads];

    for (int i = 0; i < total_threads; i++) {
        thread_ids[i] = i + 1;
        pthread_create(&threads[i], NULL, thread_function, &thread_ids[i]);
    }

    for (int i = 0; i < total_threads; i++) {
        pthread_join(threads[i], NULL);
    }

    return 0;
}