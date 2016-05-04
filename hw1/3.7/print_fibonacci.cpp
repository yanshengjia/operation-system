// created by Shengjia Yan
// @2016-3-17

#include <windows.h>
#include <iostream>
#include <stdio.h>

using namespace std;

int fibonacci(int number)
{
	if (number < 0)
	{
		cerr << "Input error!" << endl;
		return -1;
	}
	else if (number == 0)
	{
		return 0;
	}
	else if (number == 1)
	{
		return 1;
	}
	else
	{
		return fibonacci(number - 1) + fibonacci(number - 2);
	}
}


int main()
{
	int number = atoi(GetCommandLine());    // 从命令行接受参数

	for (int i = 0; i<number; i++)
	{
		cout << fibonacci(i) << " ";
	}

	cout << endl;

	return 0;
}

