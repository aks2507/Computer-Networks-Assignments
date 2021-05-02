# Computer-Networks-Assignments
## Assignment:1
1) Create processes sequentially; P1 -> P2 -> P3 -> P4
2) Create two processes from single process and the parent informs whenever and whichever of the children exit; P1 -> P2, P1 -> P3
3) Alternatingly post statements from two processes and use semaphores for synchronization
4) Use semaphores to read and write from and to shared memory

## Assignment:2
1) Alternate lines read by parent and child process from a file using semaphores
2) Alternate lines read by two diffrent processes from a file using semaphores
3) Create two shared memories X and Y. P1 writes to X. Then P2 reads from it, then P2 changes something, writes to Y and after that, P1 reads from Y,
   changes something and then writes to X and so on.

## Assignment:3
1) Basic pipes implementation
2) Parent sends a string, child makes some changes to it and then sends it back
3) Parent and child chatting system using threads and pipes
4) P1 sends a string, P2 makes some changes to it and then sends it back(P1 & P2 are diffrent programs)
5) P1 and P2 chatting system using threads and pipes
6) Implement a program using popen()
7) Use popen and create nested pipes; P1 <---> P2 <---> P3 <---> P4

## Assignment:4
1) Implement a program using polling
2) Implement a program using mkfifo()
3) Chat server using mkfifo()
4) Code Checker

## Assignment:5
1) Implement a program using raise()
2) Implement a program using different kinds of signals and use user signals as well; Communicate between both child and parent
3) Use two shared memories and signals to implement two way communication between two processes
4) Make a handler that nullifies the functioning of Ctrl+c
5) Make a handler that nullifies the functioning of Ctrl+c for only the first 5 times you press Ctrl+c

## Assignment:6
1) P2 receives a signal from P1 and P2 knows it is from P1
2) P1 executes P2, P3 and P4, and signals all of them at the same time as a group
3) Communicate between two threads within a process
4) Communication between two threads in two processes. P1 sends and P2 receives

## Assignment:7
1) Parent Process: S; Two other processes C1 and C2 send data to S via a fifo. Another process D sends signal to S, telling to send the message from C1 and C2 to
   either send to G1(children S\`1 and S\`2) or to G2(children A and B). There is one shared memory that tells how many times kill has been called.

## Assignment:8
1) Basic implementation of message passing system calls like msgget(),msgsnd(),msgrcv() and msgctl()
2) Peer to Peer chat using message queues
3) Print all the information reagarding the message queue
4) Sequentially send messages to diffrent processes i.e. P1 -> P2 -> P3 -> P4 -> P1 -> ......
5) Implement the Chat Server using message queues. If P1 sends a message then P2 and P3 should see it, If P2 sends a message then P1 and P3 should see it
   and If P3 sends a message then P1 and P2 should see it.

## Assignment:9
1) Use readv() and writev() in a client-server program

## Assignment:10
1) Create two groups of clients. If a client wants to send a message globally, it gets broadcasted to all the clients. If it wants to send locally, it gets broadcasted only to its group. Group the clients using gids.

## Assignment:11
1) Implement a basic device driver
2) Implement a basic character based device driver

## Assignment:12->Minor Exam questions implementation
1) Clients join a server in a circular manner and get notified by the previosly joined client that they have joined the client-server system. Any new client joins between C1 and the last client that joined the system.
2) P1 accepts the outputs of P2, P3 AND P4 as and when they are available, and then sends those outputs to P5 and P6 for printing them. P5 and P6 should get the outputs through scanf()/cin and you should not use any read(), write() commands in P1.

## Assignment:13
1) Iterative Connection Oriented Client-Server system -> Where the server directly serves the client
2) Concurrent Connection Oriented Client-Server system -> Where we use fork to create a child process of the server that serves the client
3) Multi-service Concurrent Connection Oriented Client-Server system -> Where we use execv() to run a service that serves a client
4) Multi-threaded Concurrent Connection Oriented Client-Server system -> Where we use threads to manage different clients

## Assignment:14
1) Using select() to implement connection between one sever and multiple clients. In this case, a group chat is implemented.

## Assignment:15
1) Basic implementation of UDP(Connectionless) socket programming(Client-Server)
2) Client-Server chat program implemented using UDP
3) One server handles a 2 TCP clients and a UDP Client
4) One server handles multiple TCP and UDP Clients(The program isn't complete)

## Assignment:16
1) Implementing Unix Domain sockets by passing the fd of a file being read. After sender reads half the file, it send the fd to receiver to read the other half of it

## Assignment:17
1) fd_pass: Accepting the client connections and then passing their fds to an alternate server that serves those clients
2) NITW_News: Consider the following different processes at NitwNews Channel Studio as shown in sample figure below. Editor(E) gets news from Reporters(R). Reporters type news through keyboard and send to standard output, which in turn goes to Editor. Whenever Editor receives a news, he sends the news to News Reader’s table. The News Readers N1 and N2 reads the news alternatively, i.e. arrange to display the news onto the Screen.(A news will be displayed only once onto the screen). If a News Reader notices a decimal number(port address) in the start of news content, then it gets connected to a Live telecaster(L) whose IP address is IP5, and the noticed port number. After connecting, the News Reader arranges to echo onto the screen, whatever it gets from Live telecaster. At this moment the other News Reader should not read any news as live telecast is going on. Once live telecast is over, the news reading will be done alternatively by N1 and N2 as usual. The News Reader who has got connected to live telecast should also notify the Editor, so that the editor will maintain a total count of live telecasts at any moment. This count can be seen/known by anybody in the studio. If Editor notices a news message with prefix “/d” from any of the Reporters, then it passes that Reporter to Document Writer(D). The Document Writer stores the news messages that it gets from the Reporter in a file(document). Implement all the different type of processes involved in this News Channel Studio scenario. You must have to use all the IPC (Inter Process Communication) mechanisms, 90% of I/O multiplexing, required BSD socket system calls and other suitable system calls.(This question is not possible to complete with the given set of system calls that we are taught, so don't worry if it doesn't work. It didn't work for anyone)

## Assignment:18
1) Implement the inetd server with multiple clients(TCP and UDP)
2) Implement the dynamic inetd server(not completed in this repository)

## Assignment:19
1) Hamming Codes
   1) Hamming Code generation
   2) Hamming Code error detection
2) RPC
   1) rpc_calc(For simple arithmetic operations)
   2) rpc_math(For arithmetic operations and sorting)
   3) rpc_squaring(For squaring a number)
   4) rpc_practice(Digits' sum)
   5) test(Just a test progrom to see if rpcgen was working fine or not)

## Assignment:20
1) IP header printing
2) Packet sniffing
3) IP Spoofing
4) Svayambhoo Server -> very similar to packet sniffing program, except this has to sniff only the packets that are sent to particular ports
5) Svayambhoo Server with tracer: 
   1) A Service server process Si can accept the first client and serves. After that it does not accept.   
   2) Any Service Server Process Si , can start its execution at any time on a different sfdi. 
   3) A Tracer process T observes a Service server process Si serving the first client. Then it sends that service port number to Ingenerate Super Server ( ISS ) so that ISS can start accepting the second client onwards for that service request.
   4) After receiving a service port number from T, the ISS only accepts service requests for any of the  Service server processes and passes the accepted clients to the corresponding Si
   5) Any client Ci can also get service port numbers from T, and can get connected for the service.
   6) Implement the processes ISS, T , Si  , Ci. See image in the folder for further clarification
   7) In this case, the code is not completed

## Assignment:21
1) A Client Process sends an enquiry message to Server computer system, asking for what are all the service server processes available in it. All the service server processes will receive this message and each one of them replies to the client about its service information (All of them may not send replies at the same time). The client now connects to the required service sever process. A client can connect to multiple server processes. Code to implement all processes. Check the image in the folder for further clarification.(The code for this is not perfect)
2) NITW Passport(This question was not solved by me, but I am still putting it up here so that one can try, if they want): Assume that a process S in a server system is providing passport verification service as follows: client processes C1, C2, C3,…Ci, which are running in the same computer system, contact the sever process S, and sends a message containing its name(Aabhaas Das), postal address (eg, H-No : 10 -12- 45, Fathima Nagar, Warangal). Names will be different but postal address is same. A process called Verifier V, is also running in the same computer system of S. Now the server process S makes arrangements, so that process V can verify the details of a Ci. The process V has to send the same details (i.e. name, postal address) to client processes and should get a confirmed message from concerned Ci. Then it informs S about the result of verification. Depending on the result of verification, process S either can issue or deny passport to Ci. The process V and a process Ci should have to use only one descriptor. (Hint: no need of signals or any IPC)

## Assignment:22
1) Printing ARP Header
2) Printing Ethernet frame

## Mid Semester Laboratory Exam Quesitons
1) All in one server(Refer the image in the folder for clarification)
2) Training and Placement Server(Refer the image in the folder for clarification)

## End Semester Laborartory Exam Question
There was a set of several questions, out of which, I got one. I will be putting both the pdfs in the folder as well, so that one might refer the questions, if they wish. My question was as follows.(It was the 3rd question)
1) Nitnetd Super Server(Privileged): Nitnetd super server is similar to inetd super server except on the limit of maximum number of clients for a particular service. Assume that Nitnetd server provides services S1, S2 on ports say 1, 2. The Nitnetd server can accept a maximum of 2 clients for S1 service on port 1, maximum of 3 clients for S2 service on port2. Once the maximum limit reaches on a port , Nitnetd will be rejecting connection requests of clients, until the running S1, S2 server processes exit after servicing some of the clients. Suppose at a moment 2 clients are being served by 2 server processes of S1. Then Nitnetd keeps rejecting of connection requests for port 1. Now, if any one S1 server process completes its service and exits, then Nitnetd can accept one client for S1 service on port1. At any time, if Nitnetd is not able to service clients because of maximum limit, it notifies to another side by server N2, which will be serving any number of the clients. But a Client should be given first preference to get connected to Nitnetd. ( As Nitnetd is the main server , assume that it provides speedy service connection than that of N2. ) Implement all the processes in this scenario. Hint : After all, the Service server processes are children of Nitnetd super server process.

# -----THE END-----
