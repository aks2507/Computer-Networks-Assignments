#include<stdio.h> 
#include <unistd.h> //for fork system call
#include <sys/types.h>
#include <stdlib.h> //for exit system call
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
const char* name2="s2";  // named semaphore

void p2(){
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
  	sem_wait(s1);

  	while(count--){
  		printf("%s  p2 -> reading from X\n" , str2);

  		strcpy(str1 , strcat(str2 , offset));
  		printf("%s   writing in Y\n", str2 );

  		sem_post(s2);
       	sem_wait(s1);
  	}
  	sem_post(s2);
  	shmdt(str1);
  	shmdt(str2);  

  	shmctl(shmid1,IPC_RMID,NULL);
  	shmctl(shmid2,IPC_RMID,NULL);  

 }


int main(){
	p2();
	return 0;
}
