#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

int sum;

void *runner(void *param);

int main(int argc, char * argv[])
{
    pthread_t tid;
    pthread_attr_t attr;
    int ret;
    
    //void *p = &ret;

    pthread_attr_init(&attr);
    pthread_create(&tid, &attr, runner, argv[1]);

    //pthread_join(tid, p);
    pthread_join(tid, (void *)&ret);

    printf("sum = %d, ret = %d\n", sum, ret);
}

void *runner(void *param)
{
    int i, upper = atoi(param);
    sum = 0;

    for(i = 0; i <= upper; i++)
        sum += i;
    
    pthread_exit((void *)75);
}

