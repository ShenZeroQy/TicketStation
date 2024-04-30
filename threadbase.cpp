#include "pch.h"
#include "threadbase.h"

/**********global variables**/

 int index=0;
 //atomic_bool Tas_state=0;
 //atomic_bool Ttas_state = 0;
 atomic_bool Queue_state[Thread_Num];
 atomic_int Queue_tail;

 long long chrononanoclock()
 {
	 chrono::time_point<chrono::steady_clock> now = chrono::steady_clock::now();
	 auto last = chrono::duration_cast<chrono::microseconds>(now - chronostart);
	 return last.count();
 }
/** ThreadLock ***********************************/
//ThreadLock::ThreadLock()
//{
//	seq = NULL;
//	flag = NULL;
//	return;
//}
//void ThreadLock::ThreadLock_init(int num)
//{
//	shared_thread_num = num;
//	seq = (int*)malloc(sizeof(int)*num);
//	flag= (char*)malloc(sizeof(char)*num);
//	for (int i = 0; i < num; i++)
//	{
//		seq[i] = 0;
//		flag[i] = 0;
//	}
//	return;
//}
//
//
//ThreadLock::~ThreadLock()
//{
//	free (seq);
//	free (flag);
//	return;
//}

//DataStaion::~DataStaion()
//{
//
//}
/*ThreadBase***************************************/
ThreadBase::ThreadBase()
{
	tid = 0;
	return;
}
void ThreadBase::ThreadBase_init(int id,char* f)
{
	tid = id;
	filename = f;
	count = 0;
	return;
}

void ThreadBase::Say_hello(int c)
{
	cout << "Hello,I am thread "<<c<<endl;
	return;
}
void ThreadBase::wait(int t)
{
	clock_t c = clock();

	while (clock() - c < t)
		;
	return;
}
//void ThreadBase::adder(int a)
//{
//	
//	while (a > count)
//	{
//		clock_t c = clock();
//		lock.Lock(tid);
//		index++;
//		count++;
//		while (clock() - c < 10)
//			;
//		lock.UnLock();
//	}
//}
//void ThreadBase::trace(TicketServiceStation* TS )
//{
//	//tid = id;
//	int rep = Tast_num / Thread_Num;
//	int i;
//	recorder.open(filename);
//	while (rep--)
//	{
//#ifdef CULock
//	//´ÖÁ£¶ÈËø
//		lock.Lock(tid);
//#endif
//		{
//			i = rand()%100;
//			if (i < Inqpc)//²é
//			{
//				long preTime = clock() - clock_0;
//				//TS.inquiry(1,1,1);
//				long postTime = clock() - clock_0;
//				string outmsg = "chaxun";
//
//				//cout << outmsg << endl;
//			}
//			else if (i < Inqpc + Buypc)//Âò
//			{
//
//			}
//			else if (i < Inqpc + Buypc + Retpc)//ÍË
//			{
//
//			}
//			else
//			{
//				cout << "err from rep(i)="<<i<<endl;
//				break;
//			}
//		}
//
//
//#ifdef CULock
//		lock.UnLock();
//#endif
//	}
//	recorder.close();
//	cout << "end trace";
//	return;
//}
void ThreadBase::trace()
{
	srand(tid);
	//tid = id;
	int rep = Tast_num / Thread_Num;
	int i;
	//cout << filename;
	recorder.open(filename);
	long long preTime;
	long long postTime;
	while (rep--)
	{

#ifdef CULock
		lock.Lock(tid);
		//´ÖÁ£¶ÈËø
#endif

		int route = randeng(Route_num);
		int departure = randeng(Stationplusr_num);
		int arrival = randeng(Stationplusr_num);
		if (departure > arrival)//swap
		{
			int rem = departure;
			departure = arrival;
			arrival = rem;
		}
		arrival++;

		{
			i = randeng(Inqpc+Buypc+Retpc);
			if (i < Inqpc&&TS->DS.saleall[route]!=0)//²é
			{
				preTime = chrononanoclock();
				int leftTicket=(*TS).inquiry(route,departure,arrival);
				postTime = chrononanoclock();
#ifdef IOLock
				lock.Lock(tid);
#endif
#ifdef GUItrace
				CombineTrace <<preTime << " " << postTime << " " << tid << " " << "RemainTicket" << " "
					<<leftTicket << " " << route+1 <<" " << departure+1 << " " << arrival+1<<endl;
#endif	
#ifdef IOLock
				lock.UnLock();
#endif	
#ifdef LocalTrace
				recorder << preTime << " " << postTime << " " << tid << " " << "RemainTicket" << " "
					<< leftTicket << " " << route+1 << " " << departure+1 << " " << arrival+1 << endl;
#endif
			}
			else if (i < Inqpc + Buypc)//Âò
			{
				preTime = chrononanoclock();
				Ticket ticket = (*TS).buyTicket("aperson",route, departure, arrival);
				//wait(randeng(10));
				postTime = chrononanoclock();
#ifdef IOLock
				lock.Lock(tid);
#endif
				if (ticket.tid > -1)
				{
					//success
					TS->sold_tickets.push_back(ticket);
#ifdef GUItrace
					CombineTrace << preTime << " " << postTime << " " << tid << " " <<
						"TicketBought" << " " << ticket.tid << " " << "aperson" << " " << ticket.route+1 <<
						" " << ticket.coach + 1 << " " << ticket.departure + 1 << " " << ticket.arrival + 1 << " "
						<< ticket.seat + 1 << endl;
#endif
#ifdef LocalTrace
					recorder << preTime << " " << postTime << " " << tid << " " <<
						"TicketBought" << " " << ticket.tid << " " << "aperson" << " " << ticket.route + 1 <<
						" " << ticket.coach + 1 << " " << ticket.departure + 1 << " " << ticket.arrival + 1 << " "
						<< ticket.seat + 1 << endl;
#endif
				}
				else
				{
					//fail  to buy
#ifdef GUItrace
					CombineTrace << preTime << " " << postTime<< " " << tid << " " << "TicketSoldOut"
						<< " " << route + 1 << " " << departure + 1 << " " << arrival + 1 << endl;
#endif
#ifdef LocalTrace
					recorder << preTime << " " << postTime << " " << tid << " " << "TicketSoldOut"
						<< " " << route + 1 << " " << departure + 1 << " " << arrival + 1 << endl;
#endif
				}
#ifdef IOLock
				lock.UnLock();
#endif	
			}
			else if (i < Inqpc + Buypc + Retpc)//ÍËÆ±
			{
				int r = (*TS).sold_tickets.size();
				if (r)//ÓÐÆ±¿ÉÒÔÍË
				{
					r = randeng(r);
					Ticket ticket = TS->sold_tickets[r];//get it
					vector<Ticket>::iterator iter = TS->sold_tickets.begin();
					TS->sold_tickets.erase(iter+r);//pop 					
					//ticket.GUIaticket();//²é¿´ÍËÆ±
					preTime = chrononanoclock();
					bool succret = (*TS).refundTicket(ticket);
					postTime = chrononanoclock();
#ifdef IOLock
					lock.Lock(tid);
#endif
					if (succret)
					{
#ifdef GUItrace
						CombineTrace << preTime << " " << postTime << " " << tid << " "
							<< "TicketRefund" << " " << ticket.tid << " " << "aperson" << " "
							<< ticket.route + 1 << " " << ticket.coach + 1 << " " << ticket.departure + 1 << " "
							<< ticket.arrival + 1 << " " << ticket.seat + 1 <<endl;
#endif
#ifdef LocalTrace
						recorder << preTime << " " << postTime << " " << tid << " "
							<< "TicketRefund" << " " << ticket.tid << " " << "aperson" << " "
							<< ticket.route + 1 << " " << ticket.coach + 1 << " " << ticket.departure + 1 << " "
							<< ticket.arrival + 1 << " " << ticket.seat + 1 <<endl ;
#endif
					}
					else
					{
#ifdef GUItrace
						CombineTrace << preTime << " " << postTime << " " << tid << " " << "ErrOfRefund" << endl;
#endif
#ifdef LocalTrace
						recorder << preTime << " " << postTime << " " << tid << " " << "ErrOfRefund" << endl;
#endif
					}
#ifdef IOLock
					lock.UnLock();
#endif	
				}
			}
			else
			{
				cout << "err from rep(i)=" << i <<"from thread"<<tid<< endl;
				break;
			}
			
		}

#ifdef CULock

		lock.UnLock();
		//´ÖÁ£¶ÈËø
#endif

	}
	recorder.close();
	cout << "end trace";
	return;
}

ThreadBase::~ThreadBase()
{
	return;
}


//TASLock::TASLock()
//{
//	Tas_state = 0;
//}
//void TASLock::Lock(int pid)
//{
//	while (Tas_state.exchange(1))
//	{ }
//	cout << pid;
//		//cout << "TAS locked by"<<pid;
//}
//void TASLock::UnLock()
//{
//	/*if (!Tas_state.exchange(0))
//		cout<<"TAS unlock error";*/
//	Tas_state = 0;
//}
//TASLock::~TASLock()
//{
//}
//
//TTASLock::TTASLock()
//{
//	Ttas_state = 0;
//}
//void TTASLock::Lock(int pid)
//{
//	while(Ttas_state.load())
//	{ }
//	while (Ttas_state.exchange(1))
//	{
//	}
//	cout << pid;
//	//cout << "TAS locked by"<<pid;
//}
//void TTASLock::UnLock()
//{
//	/*if (!Tas_state.exchange(0))
//		cout<<"TAS unlock error";*/
//	Ttas_state = 0;
//}
//TTASLock::~TTASLock()
//{
//}

QueueLock::QueueLock()
{
	slot = -1;
}
void QueueLock::init_space()
{
	Queue_state[0] = 1;
	for (int i = 1; i < Thread_Num; i++)
		Queue_state[i] = 0;
	Queue_tail = 0;
}
void QueueLock::Lock(int pid)
{
	slot = Queue_tail.fetch_add(1);
	slot = slot % Thread_Num;
	while (!Queue_state[slot].load())
	{
		//exit until Queue_state[slot]==1
	}
	//cout << pid<<endl;
	//cout << "locked by"<<pid;
}
void QueueLock::UnLock()
{

	Queue_state[slot] = 0;//free current
	slot = (slot + 1) % Thread_Num;
	Queue_state[slot] = 1;//notify next
}
QueueLock::~QueueLock()
{
}
