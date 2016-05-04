// Bounded Buffer Problem
// Created by Shengjia Yan @2016-5-2

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <iostream>
#include <pthread.h>
#include <semaphore.h>
using namespace std;

typedef int buffer_item;
#define BUFFER_SIZE 5   // 缓冲区大小

// 缓冲区
// buffer[i]==0, 空位
// buffer[i]!=0, 满位
buffer_item buffer[BUFFER_SIZE];

int item = 0;   // item
int consume = 0;

int pro_index = 0;  // 生产者放item的位置
int con_index = 0;  // 消费者取item的位置

sem_t empty;   // 同步信号量，记录有多少空位，当缓冲区满了阻止生产者放item
sem_t full;    // 同步信号量，记录有多少满位，当缓冲区空了阻止消费者拿item
pthread_mutex_t the_mutex; // 互斥信号量，一次只有一个线程访问缓冲区


// 打印缓冲区
void print_buffer()
{
    for(int i=0; i<BUFFER_SIZE; i++)
    {
        cout<<buffer[i]<<" ";
    }
    cout<<endl;
}

// 生产者插入item
int insert_item()
{
    if(buffer[pro_index] == 0)
    {
        // 空位，可以插入
        buffer[pro_index] = item;
        pro_index = (pro_index+1)%BUFFER_SIZE;
        return 0;
    }
    else
    {
        // 满位，不能插入
        return -1;
    }
    
}


// 消费者移除item
int remove_item()
{
    if(buffer[con_index] != 0)
    {
        // 满位，可以消费
        consume = buffer[con_index];
        buffer[con_index] = 0;
        con_index = (con_index+1)%BUFFER_SIZE;
        return 0;
    }
    else
    {
        // 空位，不能消费
        return -1;
    }
}


// 生产者线程
void *producer(void *param)
{   
    int id = *(char*)param;
    while(1)
    {
        // 睡眠一段随机时间
        int r = random()%3+1;
        sleep(r);
        
        sem_wait(&empty);
        pthread_mutex_lock(&the_mutex);
        
        item++;
        cout<<"Producer "<<id<<" produced "<<item<<endl;
        
        if(insert_item())
        {
           cout<<"Can't insert now!"<<endl; 
        }
        else
        {
           cout<<"Insert successfully!"<<endl;
        }
        
        print_buffer();
        cout<<endl;
        
        pthread_mutex_unlock(&the_mutex);
        sem_post(&full);
    }
    pthread_exit(0);
}


// 消费者线程
void *consumer(void *param)
{   
    int id = *(char*)param;
    while(1)
    {
        // 睡眠一段随机时间
        int r = random()%3+1;
        sleep(r);
        
        sem_wait(&full);
        pthread_mutex_lock(&the_mutex);
        
        // 移除item
        if(remove_item())
        {
            cout<<"Can't consume now!"<<endl;
        }
        else
        {
            cout<<"Consume successfully!"<<endl;
        }
        cout<<"Consumer "<<id<<" consumed "<<consume<<endl;
       
        print_buffer();
        cout<<endl;
        
        pthread_mutex_unlock(&the_mutex);
        sem_post(&empty);
    }
    pthread_exit(0);
}



int main(int argc, char *argv[])
{   
    int sleeptime = atoi(argv[1]);
    int numPro = atoi(argv[2]);
    int numCon = atoi(argv[3]);
    
    
    pthread_t pro[numPro];  // 生产者线程的标识符
    pthread_t con[numCon];  // 消费者线程的标识符
    
    pthread_attr_t attr_pro[numPro];
    pthread_attr_t attr_con[numCon];
    
    // 设置线程默认属性
    for(int i=0; i<numPro; i++)
    {
        pthread_attr_init(&attr_pro[i]);
    }
    for(int i=0; i<numCon; i++)
    {
        pthread_attr_init(&attr_con[i]);
    }

    // 2. 初始化缓冲区
    for(int i=0; i<BUFFER_SIZE; i++)
    {
        buffer[i]=0;    // 初始缓冲区都为空
    }
    
    // 初始化同步信号量
    int init_empty = sem_init(&empty,1,BUFFER_SIZE);
	int init_full = sem_init(&full,1,0);

    // 初始化互斥信号量 
    int init_mutex = pthread_mutex_init(&the_mutex, NULL);
    if(init_mutex != 0)
    {
        cout<<"互斥信号量初始化失败！"<<endl;
        exit(1);
    } 
    
    // 3. 创建生产者线程
    for(int i=0; i<numPro; i++)
    {
        int ret_pro = pthread_create(&pro[i], &attr_pro[i], producer, &i);
        if(ret_pro != 0)
        {
            cout<<"生产者创建失败！"<<endl;
            exit(1);
        }
    }
    
    
    
    // 4. 创建消费者线程
    for(int i=0; i<numCon; i++)
    {
        int ret_con = pthread_create(&con[i], &attr_con[i], consumer, &i);
        if(ret_con != 0)
        {
            cout<<"消费者创建失败！"<<endl;
            exit(1);
        }
    }
    
    for(int i=0; i<numPro; i++)
    {
        pthread_join(pro[i], NULL);
    }
    
    for(int i=0; i<numPro; i++)
    {
        pthread_join(con[i], NULL);
    }
    

    // 5. 睡眠
    sleep(sleeptime);   // 单位为秒
    
    // 6. 退出
    return 0;
}
