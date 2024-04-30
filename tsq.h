#pragma once
#include <mutex>
#include <condition_variable>
#include <queue>
#include <memory>
#include <atomic>
/************************************************************/
/*
description: 改进的线程安全队列
aurthor：Qyshen
Email：shen99855@outlook.com
Data:2021/11
*/
/****************************************************************/
template<typename T>
class threadsafe_queue
{
private:
	//mutable std::mutex mut;
	mutable std::mutex enqlock ,deqlock;

	std::queue<T> data_queue;
	//std::condition_variable data_cond ;
	std::condition_variable notEmptyCondition, notFullCondition;
	
	std::atomic_int size;
	 
	int enqsize;
	int deqsize;
	const int Max_size;
public:
	threadsafe_queue()
	{}
	queue_clear()
	{
		std::lock_guard<std::mutex> lk1(enqlock);
		std::lock_guard<std::mutex> lk2(deqlock);
		while (!data_queue.empty())
		{
			data_queue.pop();
		}
		size = 0;
		enqsize = 0;
		deqsize = 0;
	}

	void push(T new_value)
	{
		std::lock_guard<std::mutex> lk(enqlock);
		while (size >= Max_size)
		{
			notFullCondition.wait(deqlock);
		}
		data_queue.push(new_value);
		size++;
		notEmptyCondition.notify_all();
	}

	void pop(T& value)
	{
		std::unique_lock<std::mutex> lk(deqlock);
		while (size <= 0)
		{
			notEmptyCondition.wait(enqlock);
		}
		value = data_queue.front();
		data_queue.pop();
		size--;
		// alter with" enqsize++ " to improve
		notFullCondition.notify_all();
	}
	bool empty() const
	{
		std::lock_guard<std::mutex> lk1(enqlock);
		std::lock_guard<std::mutex> lk2(deqlock);

		return data_queue.empty();
	}
	//程序正确性解释：
	//1.queue_clear函数用于初始化，清空真实数据queue_data,并将size清零。
	//2.enq 方法调用必须获得enqlock，并且必须等待size<Max_max(队列非满)条件。
		//enq方法执行成功后，将size加一，并且通知所有等待队列非空的线程，可以进行deq竞争
	//3.deq方法调用必须获得deqlock，并且循环等待size>0(队列非空)条件。
		//deq方法调用成功后，将size减一，并且通知所有等待队列非满的线程，可以进行enq竞争
	//4.由于采用了mutex的lock_guard机制，锁定的时间是函数执行全过程，这降低了效率。
		//事实上，竞争锁的时机（以enq为例）可以在等待非满结束后进行。
	//总结：由于互斥锁，非空非满条件，原子型size变量的三重保护，能够保证该队列线程安全。
		//但是，也许存在过度保护的现象。
	//改进策略：开放size的权限，将其用两个非原子变量实现，size=enqsize-deqsize

	void push_improve(T new_value)
	{
		std::lock_guard<std::mutex> lk(enqlock);
		while (enqsize-deqsize >= Max_size)
		{
			notFullCondition.wait(deqlock);
		}
		data_queue.push(new_value);
		if(enqsize++>0xfffffff0);
		{
			enqlock.lock();
			enqsize = enqsize - deqsize;
			deqsize = 0;
			enqlock.unlock();
		}
		notEmptyCondition.notify_all();
	}
	//由于deqsize总是小于enqsize，只需要在enqsize即将越界时,让enq过程获得一把deq锁，
		//让enqsize和deqsize 都减去deqsize的值,这样就不需要size这个原子变量了。
		//那么对应地，deq过程中的size--这一句替换成enqsize++就可以了。
};

int main()
{}

