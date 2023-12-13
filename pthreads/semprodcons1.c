#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define BUFFER_SIZE 1

int sharedInteger = 0;
int empty = 1;  // 1 means the buffer is empty, 0 means it's full
sem_t semaphore;

void *producer(void *arg) {
    for (int i = 1; i <= 100; ++i) {
        usleep(rand() % 101 * 1000);  // Sleep between 0 and 100 ms

        // Wait until buffer is empty
        sem_wait(&semaphore);

        while (!empty) {
            // Wait for producer to produce
            sem_post(&semaphore);
            // usleep(1000);  // Busy wait for 1 ms
            sem_wait(&semaphore);
        }

        sharedInteger = i;
        empty = 0;
        printf("Produced: %d\n", i);

        // Exit critical section
        sem_post(&semaphore);
    }
    pthread_exit(NULL);
}

void *consumer(void *arg) {
    for (int i = 1; i <= 100; ++i) {
        usleep(rand() % 101 * 1000);  // Sleep between 0 and 100 ms

        // Wait until buffer is full
        sem_wait(&semaphore);

        while (empty) {
            // Wait for producer to produce
            sem_post(&semaphore);
            // usleep(1000);  // Busy wait for 1 ms
            sem_wait(&semaphore);
        }

        printf("Consumed: %d\n", sharedInteger);
        empty = 1;

        // Exit critical section
        sem_post(&semaphore);
    }
    pthread_exit(NULL);
}

int main() {
    srand((unsigned int)time(NULL));  // Seed the random number generator

    // Initialize semaphore
    if (sem_init(&semaphore, 0, 1) != 0) {
        fprintf(stderr, "Error initializing semaphore.\n");
        exit(EXIT_FAILURE);
    }

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

    // Destroy the semaphore
    sem_destroy(&semaphore);

    return 0;
}
