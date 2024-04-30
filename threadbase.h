#pragma once
//https://www.jianshu.com/p/dcce068ee32b
//https://zhuanlan.zhihu.com/p/91062516
//https://blog.csdn.net/weixin_30897233/article/details/97184230?utm_medium=distribute.pc_relevant.none-task-blog-2%7Edefault%7EBlogCommendFromBaidu%7Edefault-5.no_search_link&depth_1-utm_source=distribute.pc_relevant.none-task-blog-2%7Edefault%7EBlogCommendFromBaidu%7Edefault-5.no_search_link
//#include <iostream>
//#include <thread>

#include "ticket.h"

#define Seperate_Data_Space 10
using namespace std;
/**********global variables**/
extern ofstream CombineTrace;
extern chrono::time_point<chrono::steady_clock> chronostart;
long long chrononanoclock();
//extern int index;
//extern atomic_bool Tas_state;
//extern atomic_bool Ttas_state;
extern atomic_bool Queue_state[Thread_Num];
extern atomic_int Queue_tail;
/***classes**************/
//class TASLock
//{
//public:
//	TASLock();
//	~TASLock();
//	void Lock(int pid);
//	void UnLock();
////private:
////	atomic_bool Tas_state;
//};
//class TTASLock
//{
//public:
//	TTASLock();
//	~TTASLock();
//	void Lock(int pid);
//	void UnLock();
//	//private:
//	//	atomic_bool Ttas_state;
//};
class QueueLock
{
public:
	//QueueLock(atomic_int* Qtail,atomic_bool* Qstate);
	QueueLock();
	//QueueLock(QueueLock& Q);

	~QueueLock();
	void init_space();
	void Lock(int pid);
	void UnLock();
private:
	int slot;
	//atomic_int* Queue_tail;
	//atomic_bool* Queue_state;
};
//class ThreadLock
//{
//public:
//	/*data*/
//	int shared_thread_num;
//	int* seq;
//	char* flag;
//	/*func*/
//	ThreadLock();
//	void ThreadLock_init(int);
//	int Lock(int);
//	int Unlock(int);
//	~ThreadLock();
//private:
//	int Get_seq(int);
//	int current_lock_flag;
//};

class ThreadBase  //: public ThreadLock
{
public:
	/*data*/
	int tid;
	QueueLock lock;
	ofstream recorder;
	char*filename;
	TicketServiceStation*TS;
	/*func*/
	ThreadBase();
	void ThreadBase_init(int ,char*);
	void wait(int t);
	void Say_hello(int);//”√”⁄≤‚ ‘
	//void adder(int);
	//void trace(TicketServiceStation* TD);
	void trace();

	//int tid;
	long clock_0;
	/*run func*/
	~ThreadBase();
private:
	int count;
	//TASLock lock;
	//TTASLock lock;
	int randeng(int r) {
		if (r == 0)
			return 0;
		else
			return (rand() % r);
	}
};


