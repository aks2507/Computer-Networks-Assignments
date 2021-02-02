#include <iostream> 
#include <sys/ipc.h> 
#include <sys/shm.h> 
#include <stdio.h> 
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <pthread.h> 
#include <semaphore.h> 
#include <string>
using namespace std; 

key_t k=ftok("shared",65);
int shared_id=shmget(k,1024,0666|IPC_CREAT); 
sem_t s1,s2;

void* writer(void* args)
{
    while(true)
    {
        char *s=(char*)shmat(shared_id,(void*)0,0); 
        cout<<"Write Data: ";
        // string str;
        // getline(cin,str);
        fgets(s,1024,stdin);
        cout<<"Data written in shared memory: "<<s<<"\n"; 
        // s=const_cast<char*>(str.c_str());
        shmdt(s); 
        sem_post(&s2);
        sem_wait(&s1);
    }
}

void* reader(void* args)
{
    sem_wait(&s2);
    while(true)
    {
        char *s=(char*)shmat(shared_id,(void*)0,0); 
        printf("Data read from shared memory: %s\n",s); 
        shmdt(s);
        sem_post(&s1);
        sem_wait(&s2);
    }
}

int main() 
{ 
    sem_init(&s1,0,0); 
    sem_init(&s2,0,0);
    pthread_t thread1,thread2;
    
    pthread_create(&thread1,NULL,writer,NULL); 
    pthread_create(&thread2,NULL,reader,NULL); 
    
    pthread_join(thread1,NULL); 
    pthread_join(thread2,NULL); 
    
    sem_destroy(&s1);
    sem_destroy(&s2); 
    return 0; 
}

