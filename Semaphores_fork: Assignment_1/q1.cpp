#include <iostream>
#include <sys/types.h>
#include <unistd.h>
using namespace std;

void sequentialProcessCreation(int count)
{
    if(count==4)
        return;
    cout<<"New process created from P"<<count<<"\n";
    
    pid_t c=fork();
    
    if(c==0)//forked successfully
    {
        cout<<"Child Process with pid: "<<getpid()<<"\n";
        cout<<"Parent of process with pid: "<<getpid()<<" is process with pid: "<<getppid()<<"\n";
        cout<<"OR\nP"<<count+1<<" has been created with parent as P"<<count<<"\n";
        cout<<"---------------------------\n";
        sequentialProcessCreation(count+1);
    }
    else //fork failed
    {
        sleep(3);
        cout<<"Process: "<<getpid()<<" exits\n";
    }
    
}
int main()
{
    sequentialProcessCreation(1);
    return 0;
}
