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
