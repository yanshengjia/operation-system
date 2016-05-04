// Created by Shengjia Yan@2016-5-2
#include <windows.h>
#include <iostream>
using namespace std;

const int BUFFER_SIZE = 5;		// 缓冲区长度
int ProductID = 0;					// 产品号
int ConsumeID = 0;					// 将被消耗的产品号
int in = 0;									// 产品进缓冲区时的缓冲区下标
int out = 0;								// 产品出缓冲区时的缓冲区下标
int buffer[BUFFER_SIZE];			// 缓冲区是个循环队列
bool continue_flag = true;		// 控制程序结束

// 信号量
HANDLE mutex;	// 互斥信号量，用于线程间的互斥
HANDLE full;		// 同步信号量，当缓冲区满时迫使生产者等待
HANDLE empty;	// 同步信号量，当缓冲区空时迫使消费者等待

DWORD WINAPI Producer(LPVOID);	// 生产者线程
DWORD WINAPI Consumer(LPVOID);	// 消费者线程

int main()
{
	//创建各个互斥信号
	mutex = CreateMutex(NULL, FALSE, NULL);
	full = CreateSemaphore(NULL, BUFFER_SIZE - 1, BUFFER_SIZE - 1, NULL);
	empty = CreateSemaphore(NULL, 0, BUFFER_SIZE - 1, NULL);

	//调整下面的数值，可以发现，当生产者个数多于消费者个数时，
	//生产速度快，生产者经常等待消费者；反之，消费者经常等待
	const  int PRODUCERS_COUNT = 2; //生产者的个数
	const  int CONSUMERS_COUNT = 1; //消费者的个数

	//总的线程数
	const  int THREADS_COUNT = PRODUCERS_COUNT + CONSUMERS_COUNT;
	HANDLE threads[PRODUCERS_COUNT]; //各线程的handle
	DWORD producerID[CONSUMERS_COUNT]; //生产者线程的标识符
	DWORD consumerID[THREADS_COUNT]; //消费者线程的标识符

	//创建生产者线程
	for (int i = 0; i<PRODUCERS_COUNT; ++i){
		threads[i] = CreateThread(NULL, 0, Producer, NULL, 0, &producerID[i]);
		if (threads[i] == NULL) return -1;
	}

	//创建消费者线程
	for (int i = 0; i<CONSUMERS_COUNT; ++i){
		threads[PRODUCERS_COUNT + i] = CreateThread(NULL, 0, Consumer, NULL, 0, &consumerID[i]);
		if (threads[i] == NULL) return -1;
	}

	while (continue_flag){
		if (getchar()){ //按回车后终止程序运行
			continue_flag = false;
		}
	}
	return 0;
}

//生产一个产品。简单模拟了一下，仅输出新产品的ID号
void Produce()
{
	cerr << "Producing " << ++ProductID << " ... ";
	cerr << "Succeed" << endl;
}

//把新生产的产品放入缓冲区
void Append()
{
	cerr << "Inserting a product ... ";
	buffer[in] = ProductID;
	in = (in + 1) % BUFFER_SIZE;
	cerr << "Succeed" << endl;
	//输出缓冲区当前的状态
	for (int i = 0; i<BUFFER_SIZE; ++i){
		cout << i << ": " << buffer[i];
		if (i == in) cout << " <-- next Produce";
		if (i == out) cout << " <-- next Consume";
		cout << endl;
	}
}

//从缓冲区中取出一个产品
void Take()
{
	cerr << "Removing a product ... ";
	ConsumeID = buffer[out];
	buffer[out] = 0;
	out = (out + 1) % BUFFER_SIZE;
	cerr << "Succeed" << endl;
	//输出缓冲区当前的状态
	for (int i = 0; i<BUFFER_SIZE; ++i){
		cout << i << ": " << buffer[i];
		if (i == in) cout << " <-- next Produce";
		if (i == out) cout << " <-- next Consume";
		cout << endl;
	}
}

//消耗一个产品
void Consume()
{
	cerr << "Consuming " << ConsumeID << " ... ";
	cerr << "Succeed" << endl;
}

//生产者线程
DWORD WINAPI Producer(LPVOID lpPara)
{
	while (continue_flag){
		WaitForSingleObject(full, INFINITE);
		WaitForSingleObject(mutex, INFINITE);
		Produce();
		Append();
		Sleep(1500);
		ReleaseMutex(mutex);
		ReleaseSemaphore(empty, 1, NULL);
	}
	return 0;
}

//消费者线程
DWORD WINAPI Consumer(LPVOID lpPara)
{
	while (continue_flag){
		WaitForSingleObject(empty, INFINITE);
		WaitForSingleObject(mutex, INFINITE);
		Take();
		Consume();
		Sleep(1500);
		ReleaseMutex(mutex);
		ReleaseSemaphore(full, 1, NULL);
	}
	return 0;
}