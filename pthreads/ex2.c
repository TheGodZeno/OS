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

    for(int i = 0; i < *incoming; i++)
        sum += i;
    
    printf("sum == %d\n", sum);

    

    return incoming;
}

int main()
{
    pthread_t thread_ID;
    void *thread_result;
    int value;

    value = 42;

    pthread_create(&thread_ID, NULL, thread_function, &value);
    printf("%d\n", thread_ID);

    pthread_join(thread_ID, &thread_result);
    // value = 24;
    printf("thread_result == %d\n", *(int*)thread_result);

    return 0;
}
