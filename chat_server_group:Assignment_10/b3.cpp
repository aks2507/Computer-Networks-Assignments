#include<bits/stdc++.h>
#include<sys/ipc.h>
#include<sys/msg.h>
#include<stdio.h>
using namespace std;
struct msg_buf
{
	long mesg_type;
	char mesg_text[100];
};

int msgqid_group2=msgget(1731,0666|IPC_CREAT);
int msgqid_global=msgget(1732,0666|IPC_CREAT);

void* write1(void* args)
{
	msg_buf ms1,ms2;
	ms1.mesg_type=1;
	while(1)
	{
		char ms[100];
		cin.getline(ms,100);
		strcpy(ms1.mesg_text,"(B3)\0");
		strcat(ms1.mesg_text,ms);
		cout<<"Written:"<<ms1.mesg_text<<endl;
		cout<<"press 1 for G1, press 0 for Global"<<endl;
		int x;
		cin>>x;

		if(x==1)
			msgsnd(msgqid_group2,&ms1,sizeof(ms1),0);
		else if(x==0)
			msgsnd(msgqid_global,&ms1,sizeof(ms1),0);
		
		cin.ignore();
	}
}
void* read1(void* args)
{
	while(1)
	{
		msg_buf ms2;
		msgrcv(msgqid_group2,&ms2,sizeof(ms2),5,0);
		cout<<"Read local:"<<ms2.mesg_text<<endl;
	}
}
void* read_global1(void* args)
{
	while(1)
	{
		msg_buf ms2;
		msgrcv(msgqid_global,&ms2,sizeof(ms2),8,0);
		cout<<"Read global:"<<ms2.mesg_text<<endl;
	}
}
int main(){
	pthread_t write_th,read_local_th,read_global_th;

	pthread_create(&write_th,NULL,write1,NULL);
	pthread_create(&read_local_th,NULL,read1,NULL);
	pthread_create(&read_global_th,NULL,read_global1,NULL);
	
	pthread_join(write_th,NULL);
	pthread_join(read_local_th,NULL);
	pthread_join(read_global_th,NULL);
	return 0;
}