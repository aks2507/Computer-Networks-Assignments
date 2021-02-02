#include<stdio.h>  
#include<unistd.h>  
#include<fcntl.h> 
#include<stdlib.h> 
#include <semaphore.h>
#include<sys/types.h>
#include<sys/stat.h>
const char* sem1= "s1";
const char* sem2 = "s2";


void func(){

//printf("hiii");
sem_t *s1=sem_open(sem1,0);
sem_t *s2=sem_open(sem2,0);

sem_wait(s1);

char *buf= (char *) calloc(1, sizeof(char)); 
while(read(0,buf,1)==1){

  if(*buf=='\n'){
   printf("->p2\n");
   sem_post(s2);
   sem_wait(s1);
    }
  else if(*buf==' '){
    printf(" ");
  }
  
  else{
    printf("%s",buf);
  }

}
/*char str;
fgets(str,60,stdin);
printf("%s->p2\n",str);
sem_post(s2);
sem_wait(s1);*/

sem_post(s2);
}

int main(){

//printf("ye ho ja");

func();
return 0;
}
