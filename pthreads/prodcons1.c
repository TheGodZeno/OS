#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define BUFFER_SIZE 1

int sharedInteger = 0;

void *producer(void *arg) {
    for (int i = 1; i <= 100; ++i) {
        sharedInteger = i;
        //printf("Produced: %d\n", i);
        usleep(rand() % 101 * 1000);  // Sleep between 0 and 100 ms
    }
    pthread_exit(NULL);
}

void *consumer(void *arg) {
    for (int i = 1; i <= 100; ++i) {
        printf("Consumed: %d\n", sharedInteger);
        usleep(rand() % 101 * 1000);  // Sleep between 0 and 100 ms
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

    return 0;
}
