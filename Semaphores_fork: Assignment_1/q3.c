#include <stdio.h> 
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <pthread.h> 
#include <semaphore.h> 
  
sem_t s1;
sem_t s2; 

void* P1(void* arg) 
{ 
    int count=4;
    while(count--)
    {
        int c=4-count;
        printf("\nP1\n");
        printf("P1: S%d\n",2*c-1);
        printf("P1: S%d\n",2*c);
        printf("\nExiting P1\n"); 
        sem_post(&s2); 
        sem_wait(&s1);
    }
    sem_post(&s2);
}

void* P2(void* arg) 
{ 
    int count=4;
    sem_wait(&s2);
    while(count--)
    {
        int c=4-count;
        printf("\nP2\n");
        printf("P2: S%d\n",2*c-1);
        printf("P2: S%d\n",2*c);
        printf("\nExiting P2\n"); 
        sem_post(&s1);
        sem_wait(&s2);
    }
}

int main() 
{ 
    sem_init(&s1,0,0); 
    sem_init(&s2,0,0);
    pthread_t thread1,thread2;
    
    pthread_create(&thread1,NULL,P1,NULL); 
    pthread_create(&thread2,NULL,P2,NULL); 
    
    pthread_join(thread1,NULL); 
    pthread_join(thread2,NULL); 
    
    sem_destroy(&s1);
    sem_destroy(&s2); 
    return 0; 
}
