#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <poll.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/msg.h>
#include <sys/select.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <sys/un.h>
#define KEYPATH "."
#define CONTROLLEN CMSG_LEN(sizeof(int))
#define PATH "/tmp/file.socket"

int nsfd[255],ind = 0;
int usfd,sfd;

void m(char* s)
{
	printf("%s success!!\n",s);
}

struct shmstr
{
	int ports[255];
	int ind;
};

void sig_handler(int alrm)
{
	struct msghdr msg;
	struct iovec iov[1];
	char buff[1];
	iov[0].iov_base = buff;
	iov[0].iov_len = 1;
	msg.msg_iov = iov;
	msg.msg_iovlen = 1;
	msg.msg_name = NULL;
	msg.msg_namelen = 0;
	struct cmsghdr* cmptr;
	cmptr = (struct cmsghdr*)malloc(CONTROLLEN);
	cmptr->cmsg_type = SCM_RIGHTS;
	cmptr->cmsg_level = SOL_SOCKET;
	cmptr->cmsg_len = CONTROLLEN;
	msg.msg_control = cmptr;
	msg.msg_controllen = CONTROLLEN;
	*(int*)CMSG_DATA(cmptr) = sfd;
	if(sendmsg(usfd,&msg,0)<0)
		perror("fd_send");
	else
		printf("File DEscriptor passed\n");
}

int main(int argc, char const *argv[])
{
	key_t k = ftok(KEYPATH,65);
	if(k<0)
	{
		perror("Could not create");exit(0);
	}
	else
		m("ftok");
	int shmid = shmget(k,sizeof(struct shmstr),IPC_CREAT|0666);
	if(shmid<0)
	{
		perror("Could not create shared memory");exit(0);
	}
	else
		m("shmid");
	int semid = semget(k,1,IPC_CREAT|0666);
	if(semid<0)
	{
		perror("Could not create semaphore");exit(0);
	}
	else
		m("semid");
	struct shmstr* ptr = (struct shmstr*)shmat(shmid,NULL,0);
	// struct sembuf b;
	// b.sem_num = 0;
	// b.sem_op = -1;
	// b.sem_flg = 0;
	// semop(semid,&b,1);
	ptr->ports[ptr->ind] = atoi(argv[1]);
	ptr->ind++;
	// b.sem_num = 0;
	// b.sem_op = 1;
	// b.sem_flg = 0;
	// semop(semid,&b,1);
	shmdt(ptr);
	signal(SIGUSR1,sig_handler);
	if(argc<2)
	{
		printf("Usage: %s [PORT_NO]\n",argv[0]);
		exit(0);
	}
	else
		m("user_execution");
	usfd = socket(AF_UNIX,SOCK_STREAM,0);
	if(usfd<0)
	{
		perror("fd_pass_socket");
		exit(0);
	}
	else
		m("fd_pass_socket");
	struct sockaddr_un addr;
	addr.sun_family = AF_UNIX;
	strcpy(addr.sun_path,PATH);
	if(connect(usfd,(struct sockaddr*)&addr,sizeof(addr))<0)
	{
		perror("connect");
		exit(0);
	}
	else
		m("connect");
	int p;
	p = atoi(argv[1]);
	send(usfd,&p,sizeof(int),0);
	p = getpid();
	send(usfd,&p,sizeof(int),0);
	fd_set readset;
	FD_ZERO(&readset);
	FD_SET(usfd,&readset);
	int maxfp1 = usfd;

	sfd = socket(AF_INET,SOCK_STREAM,0);
	if(sfd<0)
	{
		perror("tcp_socket");
		exit(0);
	}
	else
		m("tcp_socket");
	int op = 1;
	setsockopt(sfd,SOL_SOCKET,SO_REUSEADDR|SO_REUSEPORT,&op,sizeof(op));
	struct sockaddr_in addr1;
	addr1.sin_family = AF_INET;
	addr1.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
	addr1.sin_port = htons(atoi(argv[1]));
	if(bind(sfd,(struct sockaddr*)&addr1,sizeof(addr1))<0)
	{
		perror("bind");
		exit(1);
	}
	else
		m("tcp_bind");
	if(listen(sfd,10)<0)
	{
		perror("listen");
		exit(1);
	}
	else
		printf("Listening..\n");
	FD_SET(sfd,&readset);
	maxfp1 = (sfd>maxfp1)?sfd:maxfp1;
	int r,f = 0;
	while(1)
	{
		int sto = ind;
		r = select(maxfp1+1,&readset,NULL,NULL,NULL);
		if(r>0)
		{
			if(FD_ISSET(sfd,&readset))
			{
				printf("Accepting for the first time\n");
				nsfd[ind] = accept(sfd,NULL,NULL);
				FD_SET(nsfd[ind],&readset);
				maxfp1 = (nsfd[ind]>maxfp1)?nsfd[ind]:maxfp1;
				//close(sfd);
				FD_CLR(sfd,&readset);f = 1;
				ind++;
				//sfd = -1;
			}
			else if(!f)
				FD_SET(sfd,&readset);
			if(FD_ISSET(usfd,&readset))
			{
				struct msghdr msg;
				struct iovec iov[1];
				char buff[1];
				iov[0].iov_base = buff;
				iov[0].iov_len = 1;
				msg.msg_iov = iov;
				msg.msg_iovlen = 1;
				msg.msg_name = NULL;
				msg.msg_namelen = 0;
				struct cmsghdr* cmptr;
				cmptr = (struct cmsghdr*)malloc(CONTROLLEN);
				cmptr->cmsg_type = SCM_RIGHTS;
				cmptr->cmsg_level = SOL_SOCKET;
				cmptr->cmsg_len = CONTROLLEN;
				msg.msg_control = cmptr;
				msg.msg_controllen = CONTROLLEN;int rr;
				if((rr = recvmsg(usfd,&msg,0))<0)
				{
					perror("Could not receive");
				}
				else if(rr==0)
				{
					printf("Terminating..\n");
					exit(0);
				}
				else
				{					
					nsfd[ind] = *(int*)CMSG_DATA(cmptr);
					maxfp1 = (nsfd[ind]>maxfp1)?nsfd[ind]:maxfp1;
					FD_SET(nsfd[ind],&readset);
					ind++;
				}
			}
			else
				FD_SET(usfd,&readset);
			for(int i=0;i<sto;i++)
			{
				if(FD_ISSET(nsfd[i],&readset))
				{
					printf("Set: %d\n",i);
					char buffer[50];
					int sz = read(nsfd[i],buffer,50);
					buffer[sz] = '\0';
					for(int j=0;j<sz;j++)
					{
						if(buffer[j]>='a' && buffer[i]<='z')
							buffer[j] = buffer[j]-'a'+'A';
					}
					send(nsfd[i],buffer,sz,0);
				}
				else
					FD_SET(nsfd[i],&readset);
			}
		}
	}
	shmctl(shmid,IPC_RMID,NULL);
	return 0;
}