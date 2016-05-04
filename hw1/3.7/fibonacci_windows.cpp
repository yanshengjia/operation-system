// created by Shengjia Yan
// 2016-3-17


#include <iostream>
#include <windows.h>
#include <stdio.h>
#include <string>
using namespace std;

void main(int argc, char *argv[])
{
	char command[256];

	scanf_s("%s", command, 256);

	// 将格式化后的数据放入 command 缓冲区
	// sprintf_s(command, "print_fibonacci.exe %d", atoi(argv[1]));

	// https://msdn.microsoft.com/en-us/library/windows/desktop/ms682512(v=vs.85).aspx
	STARTUPINFO si;
	PROCESS_INFORMATION pi;

	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);
	ZeroMemory(&pi, sizeof(pi));

	if (argc != 2)
	{
		printf("Usage: %s [cmdline]\n", argv[0]);
	}

	// [项目] -> [属性] -> [配置属性] -> [常规] -> [unicode改成未设置]， 解决 char* 和 LPWSTR 的转化问题
	// Start the child process. 
	if (!CreateProcess("print_fibonacci.exe",   // No module name (use command line)
		command,        // Command line
		NULL,           // Process handle not inheritable
		NULL,           // Thread handle not inheritable
		FALSE,          // Set handle inheritance to FALSE
		0,              // No creation flags
		NULL,           // Use parent's environment block
		NULL,           // Use parent's starting directory 
		&si,            // Pointer to STARTUPINFO structure
		&pi)           // Pointer to PROCESS_INFORMATION structure
		)
	{
		printf("CreateProcess failed (%d).\n", GetLastError());
	}
	// Wait until child process exits.
	WaitForSingleObject(pi.hProcess, INFINITE);

	// Close process and thread handles. 
	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);

	system("pause");

	return;
}
