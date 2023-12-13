#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <semaphore.h>
#include <unistd.h>

#define SEM_NAME "/my_semaphore"

int main() {
    // Open the named semaphore
    sem_t *sem = sem_open(SEM_NAME, 0);

    // Consumer logic
    for (int i = 1; i <= 10; ++i) {
        sem_wait(sem);  // Wait for producer
        printf("Consumed: %d\n", i);
        sem_post(sem);  // Signal producer
        usleep(rand() % 500000);  // Sleep between 0 and 500 ms
    }

    // Clean up
    sem_close(sem);
    sem_unlink(SEM_NAME);  // Unlink the semaphore

    return 0;
}
