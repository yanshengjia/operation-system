// homework 4.11
// created by Shengjia Yan
// @2016-3-21

#include <iostream>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <string>
#include <stdlib.h>
using namespace std;

#define MAX_SIZE 256

// 全局数组
int fibonacci[MAX_SIZE];

// 计算fibonacci[number]的值
int CalculateFibonacci(int number)
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
        return CalculateFibonacci(number-1) + CalculateFibonacci(number-2);
    }
}


// 产生fibonacci数组
// 执行新线程
void *GenerateFibonacci(void *param)
{   
    int number = atoi((char *)param);
    
    for(int i=0; i<number; i++)
    {
        fibonacci[i] = CalculateFibonacci(i);
    }
    
    pthread_exit(0);
}


int main()
{
    memset(fibonacci, 0, sizeof(fibonacci));
    
    char numberStr[MAX_SIZE];
    
    cout<<"Enter the number for the fibonacci sequence : "<<endl;
    scanf("%s", numberStr);
    
    int number = atoi(numberStr);
    
    if(number < 0)
    {
        cerr<<"number must < 0!"<<endl;
        return -1;
    }
    
    pthread_t tid;          // 线程标志符
    pthread_attr_t attr;    // 线程属性
    
    // 初始化线程属性
    pthread_attr_init(&attr);
    
    // 创建新线程，产生fibonacci序列
    int ret = pthread_create(&tid, &attr, GenerateFibonacci, numberStr);
    
    if(ret!=0)  
    {  
        // 创建新线程出错
        cout<<"Create pthread error!"<<endl;
        return -1;  
    }  
    
    // 等待子进程结束
    pthread_join(tid, NULL);
    
    // 父进程输出fibonacci数列
    for(int i=0; i<number; i++)
    {
        cout<<fibonacci[i]<<" ";
    }
    cout<<endl;
    
    
    return 0;
}