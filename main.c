#include "philosophers.h"
#include <pthread.h>
#include <time.h>
int mails = 0;
pthread_mutex_t mutex;

void    *foo(void *arg)
{
    for(int i = 0; i < 100000000; i++)
    {
        pthread_mutex_lock(&mutex);
        mails++;
        pthread_mutex_unlock(&mutex);
    }
    return NULL;
} 
int main()
{
    pthread_t th[4];
    pthread_mutex_init(&mutex, NULL);
    for (int i = 0 ; i < 4 ; i++){
        if (pthread_create(th + i, NULL , &foo, NULL))
            perror("failed to create a thread");
        printf("thread %d has started\n",i);
    }
    for (int i = 0 ; i < 4 ; i++){
        if (pthread_join(th[i],NULL))
            perror("failed to join a thread");
        printf("Thread %d has finished sucessfully\n",i);
    }
    printf("%d\n",mails);
    pthread_mutex_destroy(&mutex);
    return (0);
}
