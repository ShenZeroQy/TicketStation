#include "pch.h"
#include "ticket.h"

atomic_long Global_tid=1;
DataStaion gds;


Ticket::Ticket()
{
	tid = 0;
}


Ticket::~Ticket()
{
}


TicketServiceStation::TicketServiceStation()
{
	
}




TicketServiceStation::~TicketServiceStation()
{
}
/**DataStaion*******************************/
DataStaion::DataStaion()
{
	//initallize Inquire_data
	for (int i = 0; i < Route_num; i++)
	{
		for (int k = 0; k < Stationplusr_num; k++)
			Tickets[k][i] = Full_train_num;
		//for (int j = 0; j < Stationplusr_num; j++)
	}
}
//int DataStaion::seat(int* pick, int diff,int* C, int* S)
//{
//
//	int min = Full_train_num;
//	for (int i = 0; i < diff; i++)
//	{
//		if ((pick[i] < 0) || (pick[i] >= Full_train_num))
//			return 0;
//		if (min < pick[i])
//		{
//			min = pick[i];
//		}
//	}
//	*C = min / Coach_num;
//	*S = min % Coach_num;
//	return min;
//}
int DataStaion::seat(int* pick, int diff)
{

	int min = Full_train_num;
	for (int i = 0; i < diff; i++)
	{
		if ((pick[i] < 0) || (pick[i] > Full_train_num))
			return 0;
		if (min > pick[i])
		{
			min = pick[i];
		}
	}

	return min;
}

//int DataStaion::Fission(int r,int origin, int destination)
////ensure 0<origin<destination<Stationplusr_num
////only consider one trip
//{
//	if (origin >= Stationplusr_num || origin < 0)
//		return -1 ;
//	if (destination >= Stationplusr_num || destination < 0)
//		return -1 ;
//	if (origin > destination)//swap
//		return(Fission(r, destination,origin));
//	int sep = origin;
//	int flag = 0;
//	for (; sep > 0&&flag==0; sep--)
//		if (Inquire_data[r][sep][destination] > 0)
//		{
//			flag = 1;
//			break;
//		}
//	if (flag == 0)//found none
//	{
//		if (Inquire_data[r][0][Stationplusr_num - 1] <= 0)
//			return 0;
//		else 
//		{
//			Inquire_data[r][0][Stationplusr_num - 1]--;
//			Inquire_data[r][destination][Stationplusr_num - 1]++;
//			Inquire_data[r][origin][destination]++;
//			Inquire_data[r][0][origin]++;
//			return 1;
//		}
//	}
//	else//found sep
//	{
//		Inquire_data[r][sep][destination]--;
//		Inquire_data[r][origin][destination]++;
//		Inquire_data[r][sep][origin]++;
//		return 1;
//	}
//
//}
//int DataStaion::Ask(int r, int origin, int destination)
////ensure 0<origin<destination<Stationplusr_num
////only consider one trip
//{
//	if (origin >= Stationplusr_num || origin < 0)
//		return -1;
//	if (destination >= Stationplusr_num || destination < 0)
//		return -1;
//	if (origin > destination)//swap
//		return(Ask(r, destination, origin));
//
//	int flag = 0;
//	for (int i = origin; i >= 0 && flag == 0; i--)
//		for (int j = destination; j < Stationplusr_num; j++)
//			if (Inquire_data[r][i][j])
//			{
//				flag = 1;
//				break;
//			}
//	return flag;
//}
DataStaion::~DataStaion()
{

}