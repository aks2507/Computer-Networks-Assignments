#include<stdio.h>  
#include<unistd.h>  
#include<fcntl.h> 
#include<stdlib.h> 
#include <semaphore.h>
#include<sys/wait.h>
#include<sys/types.h>
#include<sys/stat.h>
const char* sem1= "s1";
const char* sem2 = "s2";

void p1(int fd){

sem_t *s1=sem_open(sem1,O_EXCL);
sem_t *s2=sem_open(sem2,O_EXCL);
//printf("yoooo");

char *buf= (char *) calloc(1, sizeof(char)); 
while(read(fd,buf,1)==1){

  if(*buf=='\n'){
   printf("->p1\n");
   sem_post(s1);
   sem_wait(s2);
   
  }
  else if(*buf==' '){
    printf(" ");
  }
  
  else{
    printf("%s",buf);
  }

}

}

int main()
{
	sem_t *s1 = sem_open(sem1,O_CREAT|O_EXCL,0666,0);
        sem_t *s2 = sem_open(sem2,O_CREAT|O_EXCL,0666,0);
        int fd=open("file.txt",O_RDONLY);
        dup2(fd,0);
        pid_t c=fork();
        if(c>0){
        //wait(NULL);
        p1(fd);
        }
        else{
          execv("./q2_p2",NULL);
        }
         sem_unlink(sem1);
         sem_unlink(sem2); 
	return 0;
}
