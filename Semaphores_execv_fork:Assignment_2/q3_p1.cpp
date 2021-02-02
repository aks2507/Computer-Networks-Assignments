#include<stdio.h> 
#include <unistd.h> 
#include <sys/types.h>
#include <stdlib.h> 
#include <iostream>
#include <sys/wait.h>
#include <sys/ipc.h> 
#include <sys/shm.h> 
#include <fcntl.h>
#include <pthread.h> 
#include <semaphore.h> 
#include <string.h>
using namespace std;

const char* name1="s1";
const char* name2="s2"; 

void p1(){
	sem_t* s1=sem_open(name1,O_EXCL);
	sem_t* s2=sem_open(name2,O_EXCL);
		
	key_t key1 = ftok("X",65); 
  	int shmid1 = shmget(key1,1024,0666|IPC_CREAT); 
  	char *str1 = (char*) shmat(shmid1,(void*)0,0); 
  
  	key_t key2 = ftok("Y",65); 
  	int shmid2 = shmget(key2,1024,0666|IPC_CREAT); 
  	char *str2 = (char*) shmat(shmid2,(void*)0,0); 

  	char * offset = "a";
  	int count = 5;
  	while(count --){
  		printf("%s  p1 -> reading from Y\n" , str1);

  		strcpy(str2 , strcat(str1 , offset));
  		printf("%s   writing in X\n", str1 );
  		sem_post(s1);
		sem_wait(s2);

  	}

    shmdt(str1);
  	shmdt(str2);

  	shmctl(shmid1,IPC_RMID,NULL);
  	shmctl(shmid2,IPC_RMID,NULL);  

}


int main(){
    sem_t *mutex1 = sem_open(name1,O_CREAT|O_EXCL,0666,0);
    sem_t *mutex2 = sem_open(name2,O_CREAT|O_EXCL,0666,0);

    pid_t c=fork();
    if(c>0){
    	p1();
    }
    else{
        execv("./q3_p2",NULL);
    }
    sem_unlink(name1);
    sem_unlink(name2); 
	return 0;
	
}
