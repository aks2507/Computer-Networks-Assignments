// fd passing sender side
int send_fd(int fd, int fd_to_send) 
{
    struct iovec iov[1];
    char ptr[2] = {0, 0};
    int newfd;

    struct msghdr m;

    iov[0].iov_base = ptr;
    iov[0].iov_len = 2;
    m.msg_iov = iov;
    m.msg_iovlen = 1;
    m.msg_name = NULL;
    m.msg_namelen = 0;

    cmptr = malloc(CONTROLLEN);
    cmptr->cmsg_level = SOL_SOCKET;
    cmptr->cmsg_type = SCM_RIGHTS;
    cmptr->cmsg_len = CONTROLLEN;
    m.msg_control = cmptr;
    m.msg_controllen = CONTROLLEN;
    *(int *)CMSG_DATA(cmptr) = fd_to_send;

    if(sendmsg(fd, &m, 0) < 0) 
	{
        perror("sendmsg");
        return -1;
    }

    return 0;
}

int serv_listen() 
{
    int fd, len;
    struct sockaddr_un u;

    if((fd = socket(AF_UNIX, SOCK_STREAM, 0))<0) {
        perror("socket");
        exit(1);
    }

    unlink(path);
    u.sun_family = AF_UNIX;
    strcpy(u.sun_path, path);
    len = offsetof(struct sockaddr_un, sun_path) + strlen(path);
    if(bind(fd, (struct sockaddr*)&u, len)<0) 
    {
        perror("bind");
        exit(1);
    }

    if(listen(fd,10)<0) 
    {
        perror("listen");
        exit(1);
    }

    return fd;
}

// fd passing acceptor side
int recv_fd(int fd) {

    struct iovec iov[1];
    char ptr[2];
    int newfd;

    struct msghdr m;

    iov[0].iov_base = ptr;
    iov[0].iov_len = sizeof(ptr);
    m.msg_iov = iov;
    m.msg_iovlen = 1;
    m.msg_name = NULL;
    m.msg_namelen = 0;

    
    cmptr = malloc(CONTROLLEN);
    m.msg_control = cmptr;
    m.msg_controllen = CONTROLLEN;

    if(recvmsg(fd, &m, 0) < 0) 
    {
        perror("recvmsg");
        return -1;
    }

    newfd = *(int *)CMSG_DATA(cmptr);

    return newfd;
}


int cli_conn(char *name) 
{
    int fd, len;
    struct sockaddr_un u, su;

    if((fd = socket(AF_UNIX, SOCK_STREAM, 0)) < 0) {
        perror("socket");
        exit(1);
    }
    int opt=1;
    if(setsockopt(fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,&opt, sizeof(opt))) 
    { 
        perror("setsockopt"); 
        exit(1); 
    }

    unlink(name);
    u.sun_family = AF_UNIX;
    strcpy(u.sun_path, name);
    len = offsetof(struct sockaddr_un, sun_path) + strlen(name);
    if(bind(fd, (struct sockaddr*)&u, len)) 
    {
        perror("bind");
        exit(1);
    }

    su.sun_family = AF_UNIX;
    strcpy(su.sun_path, path);
    len = offsetof(struct sockaddr_un, sun_path) + strlen(path);
    if(connect(fd, (struct sockaddr*)&su, len) < 0) 
    {
        perror("connect");
        exit(1);
    }

    return fd;
}

// TCP server side
int inet_serv_listen(int port)
{
    struct sockaddr_in servaddr;
    int sfd, len;
    int opt=1;
    if((sfd = socket(AF_INET, SOCK_STREAM, 0))==0) 
    {
        perror("socket");
        exit(1);
    }
    if(setsockopt(sfd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,&opt, sizeof(int))) 
    { 
        perror("setsockopt"); 
        exit(1); 
    } 
    servaddr.sin_family=AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY); 
    servaddr.sin_port = htons(port);
    if(bind(sfd, (struct sockaddr*)&servaddr,sizeof(servaddr))<0)
    {
        perror("bind");
        exit(1);
    } 

    if(listen(sfd,10)<0)
    {
        perror("listen");
        exit(1);
    }
    return sfd;
}

// TCP client side
int cli_conn_tcp(int port)
{
	struct sockaddr_in servaddr; 

    int n, len; 
	int opt=1;
    // Creating socket file descriptor 
    if ((sfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) { 
        printf("socket creation failed"); 
        exit(0); 
    } 
  	
	if(setsockopt(usfd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,&opt, sizeof(int))) 
    { 
        perror("setsockopt"); 
        exit(1); 
    }
	
    memset(&servaddr, 0, sizeof(servaddr)); 
  
    // Filling server information 
    servaddr.sin_family = AF_INET; 
    servaddr.sin_port = htons(port); 
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    if (connect(sfd, (struct sockaddr*)&servaddr,sizeof(servaddr)) < 0) 
	{ 
        printf("\n Error : Connect Failed \n"); 
    }

	return sfd;
}

// UDP server side
int inet_udp_serv_listen(int port)
{
    struct sockaddr_in servaddr;
    int usfd;
    int opt=1;
    if((usfd=socket(AF_INET,SOCK_DGRAM,0))<0)
    {
        perror("UDP socket");
        exit(1);
    }
    if(setsockopt(usfd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,&opt, sizeof(int))) 
    { 
        perror("setsockopt"); 
        exit(1); 
    }

    servaddr.sin_family=AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY); 
    servaddr.sin_port = htons(port);
    if(bind(usfd, (struct sockaddr*)&servaddr, sizeof(servaddr))<0)
    {
        perror("UDP bind");
        exit(1);
    }

    return usfd;
}

// UDP client side
int cli_conn_udp(int port)
{
	struct sockaddr_in servaddr;

    int n, len; 
	int opt=1;
    // Creating socket file descriptor 
    if ((usfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) 
	{ 
        printf("socket creation failed"); 
        exit(0); 
    } 
  	
	if(setsockopt(usfd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,&opt, sizeof(int))) 
    { 
        perror("setsockopt"); 
        exit(1); 
    }
	
    memset(&servaddr, 0, sizeof(servaddr)); 

    servaddr.sin_family = AF_INET; 
    servaddr.sin_port = htons(port); 
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
}
