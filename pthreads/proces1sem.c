#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <semaphore.h>
#include <unistd.h>

#define SEM_NAME "/my_semaphore"

int main() {
    // Create a named semaphore
    sem_t *sem = sem_open(SEM_NAME, O_CREAT | O_EXCL, 0644, 1);

    if (sem == SEM_FAILED) {
        perror("sem_open");
        exit(EXIT_FAILURE);
    }

    // Producer logic
    for (int i = 1; i <= 10; ++i) {
        sem_wait(sem);  // Wait for consumer
        printf("Produced: %d\n", i);
        sem_post(sem);  // Signal consumer
        usleep(rand() % 500000);  // Sleep between 0 and 500 ms
    }

    // Clean up (no sem_unlink here)
    sem_close(sem);

    return 0;
}
