// Operation System 3.6
// created by Shengjia Yan
// 2016年3月16日 下午2:51

#include <iostream>
#include <unistd.h>  
#include <stdio.h>
#include <sys/types.h>

using namespace std;

int fibonacci(int number)
{
    if(number < 0)
    {
        cerr<<"Input error!"<<endl;
        return -1;
    }
    else if(number == 0)
    {
        return 0;
    }
    else if(number == 1)
    {
        return 1;
    }
    else
    {
        return fibonacci(number-1) + fibonacci(number-2);
    }
}

int main ()   
{   
    int number;
    cout<<"Enter the number: "<<endl;
    cin>>number;
    
    pid_t pid;
    
    pid = fork();
     
    if (pid < 0)
    {
        // 出现错误
        cerr<<"Fork failed!"<<endl;
        exit(-1);
    }
    else if(pid == 0)
    {
        // 子进程
        for(int i=0; i<number; i++)
        {
            cout<<fibonacci(i)<<" ";
        }
        cout<<endl;
    }
    else 
    {
        // 父进程
        wait(NULL);
        cout<<"Child Complete!"<<endl;
        exit(0); 
    }
    
    
    return 0;  
}  