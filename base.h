#pragma once
#include<iostream>
#include<fstream>
#include<queue>
#include<atomic>
#include <thread>
#include<chrono>
#include<mutex>
#include <ctime>
#include<cmath>
#include<random>


//#define CULock 1//��������
//IOLock ��Culock����ͬʱ����
#define Record  // ���ɲ���trace
#define LocalTrace  // ����ÿ���̵߳�trace
#ifdef Record
#define IOLock 1
//IOLock ��Culock����ͬʱ����
#define GUItrace 1
#endif

#define Thread_Num 1
//�߳���Ŀ
#define Route_num 3//������Ŀ
#define Station_num 3//վ����Ŀ
#define Coach_num 3//������Ŀ
#define Seat_num 3//��λ��Ŀ

#define Stationplusr_num (Station_num-1)
#define Full_train_num (Coach_num*Seat_num)

#define Buypc 30//��Ʊ�ٷֱ�
#define Retpc 10 //��Ʊ�ٷֱ�
#define Inqpc 60//��Ʊ�ٷֱ�
#define Tast_num 100000//������Ŀ

using namespace std;



//ofstream Fos;
