#pragma once
#include "base.h"



//extern DataStaion& ds;
extern atomic_long Global_tid;

class Ticket
{
public:
	/*data*/
	long tid;
	string passenger;
	int route;
	int coach;
	int seat;
	int departure;
	int arrival;
	/*func*/
	Ticket();
	Ticket(const Ticket& t)
	{
		tid = t.tid;
		passenger = t.passenger;
		route = t.route;
		coach = t.coach;
		seat = t.seat;
		departure = t.departure;
		arrival = t.arrival;
		return;
	}
	void GUIaticket()
	{
		cout << "ticket" << tid<<" "<<route << " " << coach << " " << departure << " "
			<< arrival << " " << seat<<endl;
	}
	/*Ticket desire(string man, int dep, int arr)
	{
		Ticket t;
		t.passenger = man;
		t.departure = dep;
		t.arrival = arr;
	}
	void PreTrace();
	void EndTrace();*/

	void ticket_init(int);
	~Ticket();
};


class DataStaion
{
public:
	DataStaion();
	~DataStaion();
	long tid_current_val;
	
	mutex Rlock[Route_num];
	mutex RSRlock[Route_num][Seat_num];
	
	int seat(int* pick, int diff);
	
	int Tickets[Stationplusr_num][Route_num];
	char saleall[Coach_num] = { 0 };
	char seatrecRS[Route_num][Stationplusr_num][Full_train_num];
	char Rtickets[Stationplusr_num][Route_num][Coach_num][Seat_num] = { 0 };
	
	int  Inquire(int*Mirr,int r, int d , int a)
	{
		if (saleall[r] != 0)
			return 0;
		if (d >a)
			return -1;
		else 
		{
			int diff = a - d ;
			for (int i = 0; i < diff; i++)
				Mirr[i] = Tickets[d+i][r];
			return seat(Mirr, diff);
		}
	}
	/*Ticket  Inq_and_buy(int* Mirr,int r, int d, int a)
	{
		Ticket giveat;
		if (d >= a)
			cout << "err from :DS:Inq and buy dest exceed arrive";

		else
		{

			int diff = a - d + 1;
			for (int i = 0; i < diff; i++)
				Mirr[i] = Tickets[a + i][r];
			diff= seat(Mirr, diff,&(giveat.coach),&(giveat.seat));
		}
		return giveat;
	}
	*/
	int BuyTicket(int* Mirr, int r, int d, int a, int* C, int* S)
		//ret -1 if fail to buy
	{

		//int ret;
		//Rlock[r].lock();
		if (saleall[r] == 0)
		{
			int sid = Inquire(Mirr, r, d, a);
			if (sid > 0)
			{
				/*sid--;
				*C = sid / Seat_num;
				*S = sid % Seat_num;
				sid++;*/
				for (int j = d; j < a; j++)
				{
					Tickets[j][r]--;
				}
				//sid = Full_train_num;
				for (int i = Full_train_num - 1; i >= 0; i--)
				{
					bool foundposition = 1;
					for (int j = d; j < a; j++)
					{
						if (seatrecRS[r][j][i] != 0)
						{
							//无座
							foundposition = 0;
							break;
						}
					}
					if (foundposition)
					{
						for (int j = d; j < a; j++)
							seatrecRS[r][j][i] = 1;
						sid = i;
						break;
					}
				}
				*C = sid / Seat_num;
				*S = sid % Seat_num;
				sid++;
			}
			//Rlock[r].unlock();
			else
			{
				saleall[r] = 1;
				for (int j = 0; j < Stationplusr_num; j++)
				{
					int c; 
					int s;
					for (int i = 0; i < Full_train_num; i++)
					{
						if (seatrecRS[r][j][i] == 0)
						{
							c = i / Seat_num;
							s =i % Seat_num;
							Rtickets[j][r][c][s] = 1;
						}
					}
				}
			}
			return sid;
		}
		else
			return 0;
	
	}
	int InqRSTicketfast_Coach( int r, int d, int a, int s )
	{//return -1 if found none
		for (int i = 0; i < Coach_num; i++)
		{
			bool f = 1;
			for (int j = d; j < a; j++)
			{
				if (Rtickets[j][r][i][s] != 1)
				{
					f = 0;
					break;
				}

			}
			if (f)
			{
				return i;
			}
		}
		return -1;
	}
	int BuyRSTicket(int r, int d, int a, int s)
	{
		//return -1 if fail
		int buyc = InqRSTicketfast_Coach(r, d, a, s);
		if (buyc > -1)
		{
			for (int j = d; j < a; j++)
				Rtickets[j][r][buyc][s] = 0;
		}
		return buyc;
	}
	int InqRSTicket( int r, int d, int a, int s)
	{
		int cnt = 0;
		for (int i = 0; i < Coach_num; i++)
		{
			bool f = 1;
			for (int j = d; j < a; j++)
			{
				if (Rtickets[j][r][i][s] != 1)
				{
					f = 0;
					break;
				}

			}
			if (f)
				cnt++;
		}
		return cnt;
	}
	int InqRTicket(int r, int d, int a)
	{
		int cnt = 0;
		//for(int i=0;i<Coach_num;i++)
		for (int j = 0; j < Seat_num; j++)
		{
			cnt += InqRSTicket(r, d, a, j);

		}
		return cnt;
	}
	void RetTicket(int r, int d, int a,int c,int s)
	{
		//RSRlock[r][s].lock();
		for (int j = d; j < a;j++)
			Rtickets[j][r][c][s] = 1;
		//RSRlock[r][s].unlock();
	}
	
private:
	;;
};

extern DataStaion gds;
class TicketServiceStation
{
public:
	///*data*/
	vector<Ticket> sold_tickets;
	/*func*/
	DataStaion& DS=gds;
	/*interface*/
	Ticket buyTicket(string p, int r, int d, int a)
	{
		bool haveit = 0;
		int retsid;
		Ticket at;
		at.passenger = p;//passage
		at.arrival = a;//arr
		at.departure = d;//dep
		at.route = r;
		//first buy a new ticket
		DS.Rlock[r].lock();
		retsid= DS.BuyTicket(Mirr, r, d, a, &(at.coach), &(at.seat));
		//if (at.seat < 0 || at.seat >= Seat_num || at.route >= Route_num || at.route < 0)
		//{
		//	cout << "warn";//非法票
		//}
		DS.Rlock[r].unlock();
		/*{
			lock_guard<mutex> lg(DS.Rlock[r]);
			retsid = DS.BuyTicket(Mirr, r, d, a, &(at.coach), &(at.seat));
		}*/
		if (retsid > 0)//success
		{
			haveit = 1;
			//return at;
		}
		//try to buy a ticket refunded
		for (int i = 0; i < Seat_num&&haveit==0; i++)
		{
			retsid = DS.InqRSTicketfast_Coach(r, d, a, i);
			if (retsid > -1)//found a ticket
			{
				//lock_guard<mutex> lg(DS.RSRlock[r][i]);//getRSlock
				DS.RSRlock[r][i].lock();
				retsid = DS.BuyRSTicket(r, d, a, i);
				DS.RSRlock[r][i].unlock();
				if (retsid > -1)//get it
				{
					haveit = 1;
					at.seat = i;//seat
					at.coach = retsid;//coach
					//return at;
				}
			}
		}
		if(haveit)
			at.tid = Global_tid.fetch_add(1);//tid
		else
			at.tid = -1;//none ticket bought
		return at;
	}
	int inquiry(int r, int d, int a)
	{
		//lock free
		int cnt = 0;
		for (int i = 0; i < Seat_num; i++)
		{
			cnt += DS.InqRSTicket(r, d, a, i);
		}
		cnt = DS.InqRTicket(r, d, a);
		
		return (DS.Inquire(Mirr, r, d, a) + cnt);
		
	}
	bool refundTicket(Ticket t)
	{
		if (t.tid < 0)
			return 0;//退假票
		if (t.seat < 0 || t.seat >= Seat_num || t.route >= Route_num || t.route < 0)
		{
			cout << "warn";//非法票
			return 0;
		}
		lock_guard<mutex> lg(DS.RSRlock[t.route][t.seat]);//getRSlock
		DS.RetTicket(t.route, t.departure, t.arrival, t.coach, t.seat);
		return 1;
	}
	TicketServiceStation();
	void TicketDS_init(int routenum, int coachnum, int seatnum, int stationnum, DataStaion& d) {
		RRoute = routenum;
		RCoach = coachnum;
		RStation = stationnum;
		RSeat = seatnum;
		//DS = d;
	}
	~TicketServiceStation();
private:
	/*data*/
	int Mirr[Stationplusr_num];
	
	//queue<long> sale_ticket_fast_tid;
	int RRoute;
	int RCoach;
	int RSeat;
	int RStation;
};

