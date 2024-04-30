// periallelThread.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "pch.h"
#include <iostream>
#include"threadbase.h"
//auto start = chrono::steady_clock::now();
chrono::time_point<chrono::steady_clock> chronostart = chrono::steady_clock::now();
//DataStaion gds;
//DataStaion& ds=gds;
ofstream CombineTrace("data/trace.txt");

int main()
{
	//reset random seed here
	srand(115200);
	chrono::time_point<chrono::steady_clock> start = chrono::steady_clock::now();
	chrono::time_point<chrono::steady_clock> c = start;
	thread* Threads= new thread[Thread_Num];
	ThreadBase TB[Thread_Num];
	TicketServiceStation TS[Thread_Num];
	char fname[] = "data/trace0.txt";
	//for queuelock
	TB[0].lock.init_space();
	clock_t StartTime = clock();//timer initer
	cout << "start!\n";
	char*fn;
	chronostart = chrono::steady_clock::now();//refreshclock
	for (int i = 0; i < Thread_Num; i++)
	{
		fname[10] = char(i+48);
		fn = new char[16];
		for (int j = 0; j < 16; j++)
		{
			fn[j] = fname[j];
		}
		TB[i].ThreadBase_init(i,fn);
		TB[i].clock_0 = StartTime;
		TB[i].TS = TS + i;
		//*(Threads+i) = thread(&ThreadBase::adder, TB + i, 1000);
		*(Threads+i) = thread(&ThreadBase::trace, TB+i);

	}
	for (int i = 0; i < Thread_Num; i++)
	{
		Threads[i].join();
	}
	long runtime = clock() - StartTime;
	cout << "end processing!\n";
	cout << "runtime=" << runtime;
	CombineTrace.close();
	return 0;
}


