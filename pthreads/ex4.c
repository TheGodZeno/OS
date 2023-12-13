#include <stdio.h>
#include <pthread.h>

/*
* The function to be executed by the thread should take a
* void * parameter and return a void * result .
*/

void *thread_function(void *arg)
{
    int sum = 0;
    int *incoming = (int*)arg;

    for(int i = 0; i <= *incoming; i++)
        sum += i;
    
    printf("thread - %d, sum == %d\n", *(int*)arg, sum);

    return incoming;
}

int main()
{
    pthread_t thread_IDs[10];
    void *thread_results[10];
    int values[] = {10, 11, 12, 13, 10000, 15, 16, 17, 18, 19};

    for(int i = 0; i <10; i++)
        pthread_create(&thread_IDs[i], NULL, thread_function, values+i);

    for(int i = 0; i <10; i++)
        pthread_join(thread_IDs[i], thread_results + i);

    for(int i = 0; i <10; i++)
        printf("thread_results == %d\n", *(int*)thread_results[i]);

    return 0;
}