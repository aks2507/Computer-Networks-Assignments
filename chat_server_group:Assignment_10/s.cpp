#include<bits/stdc++.h>
#include<sys/ipc.h>
#include<sys/msg.h>
#include<stdio.h>
#include<pthread.h>
using namespace std;
struct msg_buf{
	long mesg_type;
	char mesg_text[100];
};

int msgqid_group1=msgget(1730,0666|IPC_CREAT);
int msgqid_group2=msgget(1731,0666|IPC_CREAT);
int msgqid_global=msgget(1732,0666|IPC_CREAT);

void* read_g1(void* args)
{
	msg_buf ms1,ms2,ms3,ms4;
	
	while(1)
	{
		msgrcv(msgqid_group1,&ms1,sizeof(ms1),1,0);
		cout<<"R:"<<ms1.mesg_text<<endl;
		int x=ms1.mesg_text[2]-'0';

		strcpy(ms2.mesg_text,"\0");
		strcpy(ms3.mesg_text,"\0");
		strcpy(ms4.mesg_text,"\0");

		strcpy(ms2.mesg_text,ms1.mesg_text);
		strcpy(ms3.mesg_text,ms1.mesg_text);
		strcpy(ms4.mesg_text,ms1.mesg_text);

		ms2.mesg_type=3;
		ms3.mesg_type=4;
		ms4.mesg_type=5;
		if(x==1)
		{//message recieved from c1
			msgsnd(msgqid_group1,&ms3,sizeof(ms3),0);//to c2
			msgsnd(msgqid_group1,&ms4,sizeof(ms4),0);//to c3
		}
		else if(x==2)
		{//message recieved from c2
			msgsnd(msgqid_group1,&ms2,sizeof(ms2),0);//to c1
			msgsnd(msgqid_group1,&ms4,sizeof(ms4),0);//to c3
		}
		else
		{//message recieved from c3
			msgsnd(msgqid_group1,&ms2,sizeof(ms2),0);//to c1
			msgsnd(msgqid_group1,&ms3,sizeof(ms3),0);//to c2
		}
	}
}

void* read_g2(void* args)
{
	msg_buf ms1,ms2,ms3,ms4;
	
	while(1)
	{
		msgrcv(msgqid_group2,&ms1,sizeof(ms1),1,0);
		cout<<"R:"<<ms1.mesg_text<<endl;
		int x=ms1.mesg_text[2]-'0';
		strcpy(ms2.mesg_text,"\0");
		strcpy(ms3.mesg_text,"\0");
		strcpy(ms4.mesg_text,"\0");
		strcpy(ms2.mesg_text,ms1.mesg_text);
		strcpy(ms3.mesg_text,ms1.mesg_text);
		strcpy(ms4.mesg_text,ms1.mesg_text);
		ms2.mesg_type=3;
		ms3.mesg_type=4;
		ms4.mesg_type=5;

		if(x==1)
		{//message recieved from c1
			msgsnd(msgqid_group2,&ms3,sizeof(ms3),0);//to c2
			msgsnd(msgqid_group2,&ms4,sizeof(ms4),0);//to c3
		}
		else if(x==2)
		{//message recieved from c2
			msgsnd(msgqid_group2,&ms2,sizeof(ms2),0);//to c1
			msgsnd(msgqid_group2,&ms4,sizeof(ms4),0);//to c3
		}
		else
		{//message recieved from c3
			msgsnd(msgqid_group2,&ms2,sizeof(ms2),0);//to c1
			msgsnd(msgqid_group2,&ms3,sizeof(ms3),0);//to c2
		}
	}
}

void* read_global(void* args){
	msg_buf ms1,ms2,ms3,ms4,ms5,ms6,ms7;
	
	while(1)
	{
		msgrcv(msgqid_global,&ms1,sizeof(ms1),1,0);
		cout<<"Read global:"<<ms1.mesg_text<<endl;
		int x=ms1.mesg_text[2]-'0';
		char y=ms1.mesg_text[1];

		strcpy(ms2.mesg_text,"\0");
		strcpy(ms3.mesg_text,"\0");
		strcpy(ms4.mesg_text,"\0");
		strcpy(ms5.mesg_text,"\0");
		strcpy(ms6.mesg_text,"\0");
		strcpy(ms7.mesg_text,"\0");

		strcpy(ms2.mesg_text,ms1.mesg_text);
		strcpy(ms3.mesg_text,ms1.mesg_text);
		strcpy(ms4.mesg_text,ms1.mesg_text);
		strcpy(ms5.mesg_text,ms1.mesg_text);
		strcpy(ms6.mesg_text,ms1.mesg_text);
		strcpy(ms7.mesg_text,ms1.mesg_text);

		ms2.mesg_type=3;
		ms3.mesg_type=4;
		ms4.mesg_type=5;
		ms5.mesg_type=6;
		ms6.mesg_type=7;
		ms7.mesg_type=8;

		if(x==1)
		{
			if(y=='A')
			{//message recieved from a1
				msgsnd(msgqid_global,&ms3,sizeof(ms3),0);//to c1
				msgsnd(msgqid_global,&ms4,sizeof(ms4),0);//to c3
				msgsnd(msgqid_global,&ms5,sizeof(ms5),0);//to c1
				msgsnd(msgqid_global,&ms6,sizeof(ms6),0);//to c3
				msgsnd(msgqid_global,&ms7,sizeof(ms7),0);//to c1
			}
			else{//message recieved from b1
				msgsnd(msgqid_global,&ms3,sizeof(ms3),0);//to c1
				msgsnd(msgqid_global,&ms4,sizeof(ms4),0);//to c3
				msgsnd(msgqid_global,&ms2,sizeof(ms2),0);//to c1
				msgsnd(msgqid_global,&ms6,sizeof(ms6),0);//to c3
				msgsnd(msgqid_global,&ms7,sizeof(ms7),0);//to c1
			}
		}
		else if(x==2)
		{
			if(y=='A')
			{//message recieved from a2
				msgsnd(msgqid_global,&ms2,sizeof(ms2),0);//to c1
				msgsnd(msgqid_global,&ms4,sizeof(ms4),0);//to c3
				msgsnd(msgqid_global,&ms5,sizeof(ms5),0);//to c1
				msgsnd(msgqid_global,&ms6,sizeof(ms6),0);//to c3
				msgsnd(msgqid_global,&ms7,sizeof(ms7),0);//to c1
			}
			else
			{//message recieved from b2
				msgsnd(msgqid_global,&ms3,sizeof(ms3),0);//to c1
				msgsnd(msgqid_global,&ms4,sizeof(ms4),0);//to c3
				msgsnd(msgqid_global,&ms5,sizeof(ms5),0);//to c1
				msgsnd(msgqid_global,&ms2,sizeof(ms2),0);//to c3
				msgsnd(msgqid_global,&ms7,sizeof(ms7),0);//to c1
			}
			
		}
		else
		{//message recieved from c3
			if(y=='A')
			{//message recieved from a3
				msgsnd(msgqid_global,&ms3,sizeof(ms3),0);//to c1
				msgsnd(msgqid_global,&ms4,sizeof(ms4),0);//to c3
				msgsnd(msgqid_global,&ms2,sizeof(ms2),0);//to c1
				msgsnd(msgqid_global,&ms6,sizeof(ms6),0);//to c3
				msgsnd(msgqid_global,&ms7,sizeof(ms7),0);//to c1
			}
			else
			{//message recieved from b3
				msgsnd(msgqid_global,&ms3,sizeof(ms3),0);//to c1
				msgsnd(msgqid_global,&ms4,sizeof(ms4),0);//to c3
				msgsnd(msgqid_global,&ms5,sizeof(ms5),0);//to c1
				msgsnd(msgqid_global,&ms6,sizeof(ms6),0);//to c3
				msgsnd(msgqid_global,&ms2,sizeof(ms2),0);//to c1
			}
		}
	}
}

int main()
{
	pthread_t t1,t2,t3;
	pthread_create(&t1,NULL,read_g1,NULL);
	pthread_create(&t2,NULL,read_g2,NULL);
	pthread_create(&t3,NULL,read_global,NULL);
	pthread_join(t1,NULL);
	pthread_join(t3,NULL);
	pthread_join(t2,NULL);
	return 0;
}
