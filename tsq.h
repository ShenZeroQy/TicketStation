#pragma once
#include <mutex>
#include <condition_variable>
#include <queue>
#include <memory>
#include <atomic>
/************************************************************/
/*
description: �Ľ����̰߳�ȫ����
aurthor��Qyshen
Email��shen99855@outlook.com
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
	//������ȷ�Խ��ͣ�
	//1.queue_clear�������ڳ�ʼ���������ʵ����queue_data,����size���㡣
	//2.enq �������ñ�����enqlock�����ұ���ȴ�size<Max_max(���з���)������
		//enq����ִ�гɹ��󣬽�size��һ������֪ͨ���еȴ����зǿյ��̣߳����Խ���deq����
	//3.deq�������ñ�����deqlock������ѭ���ȴ�size>0(���зǿ�)������
		//deq�������óɹ��󣬽�size��һ������֪ͨ���еȴ����з������̣߳����Խ���enq����
	//4.���ڲ�����mutex��lock_guard���ƣ�������ʱ���Ǻ���ִ��ȫ���̣��⽵����Ч�ʡ�
		//��ʵ�ϣ���������ʱ������enqΪ���������ڵȴ�������������С�
	//�ܽ᣺���ڻ��������ǿշ���������ԭ����size���������ر������ܹ���֤�ö����̰߳�ȫ��
		//���ǣ�Ҳ����ڹ��ȱ���������
	//�Ľ����ԣ�����size��Ȩ�ޣ�������������ԭ�ӱ���ʵ�֣�size=enqsize-deqsize

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
	//����deqsize����С��enqsize��ֻ��Ҫ��enqsize����Խ��ʱ,��enq���̻��һ��deq����
		//��enqsize��deqsize ����ȥdeqsize��ֵ,�����Ͳ���Ҫsize���ԭ�ӱ����ˡ�
		//��ô��Ӧ�أ�deq�����е�size--��һ���滻��enqsize++�Ϳ����ˡ�
};

int main()
{}

