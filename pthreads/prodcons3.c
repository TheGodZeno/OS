#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define BUFFER_SIZE 1

int sharedInteger = 0;
int empty = 1;  // 1 means the buffer is empty, 0 means it's full
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

void *producer(void *arg) {
    for (int i = 1; i <= 100; ++i) {
        usleep(rand() % 101 * 1000);  // Sleep between 0 and 100 ms

        // Enter critical section
        pthread_mutex_lock(&mutex);

        while (!empty) {
            // Wait for consumer to consume
            pthread_cond_wait(&cond, &mutex);
        }

        sharedInteger = i;
        empty = 0;
        printf("Produced: %d\n", i);

        // Signal consumer that a new number is ready
        pthread_cond_signal(&cond);

        // Exit critical section
        pthread_mutex_unlock(&mutex);
    }
    pthread_exit(NULL);
}

void *consumer(void *arg) {
    for (int i = 1; i <= 100; ++i) {
        usleep(rand() % 101 * 1000);  // Sleep between 0 and 100 ms

        // Enter critical section
        pthread_mutex_lock(&mutex);

        while (empty) {
            // Wait for producer to produce
            pthread_cond_wait(&cond, &mutex);
        }

        printf("Consumed: %d\n", sharedInteger);
        empty = 1;

        // Signal producer that the number has been consumed
        pthread_cond_signal(&cond);

        // Exit critical section
        pthread_mutex_unlock(&mutex);
    }
    pthread_exit(NULL);
}

int main() {
    srand((unsigned int)time(NULL));  // Seed the random number generator

    pthread_t producerThread, consumerThread;

    // Create producer thread
    if (pthread_create(&producerThread, NULL, producer, NULL) != 0) {
        fprintf(stderr, "Error creating producer thread.\n");
        exit(EXIT_FAILURE);
    }

    // Create consumer thread
    if (pthread_create(&consumerThread, NULL, consumer, NULL) != 0) {
        fprintf(stderr, "Error creating consumer thread.\n");
        exit(EXIT_FAILURE);
    }

    // Wait for threads to finish
    if (pthread_join(producerThread, NULL) != 0) {
        fprintf(stderr, "Error joining producer thread.\n");
        exit(EXIT_FAILURE);
    }

    if (pthread_join(consumerThread, NULL) != 0) {
        fprintf(stderr, "Error joining consumer thread.\n");
        exit(EXIT_FAILURE);
    }

    // Destroy the mutex and condition variable
    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond);

    return 0;
}
