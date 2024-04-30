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


//#define CULock 1//粗粒度锁
//IOLock 和Culock不能同时定义
#define Record  // 生成并发trace
#define LocalTrace  // 生成每个线程的trace
#ifdef Record
#define IOLock 1
//IOLock 和Culock不能同时定义
#define GUItrace 1
#endif

#define Thread_Num 1
//线程数目
#define Route_num 3//车次数目
#define Station_num 3//站点数目
#define Coach_num 3//车厢数目
#define Seat_num 3//座位数目

#define Stationplusr_num (Station_num-1)
#define Full_train_num (Coach_num*Seat_num)

#define Buypc 30//买票百分比
#define Retpc 10 //退票百分比
#define Inqpc 60//查票百分比
#define Tast_num 100000//测试数目

using namespace std;



//ofstream Fos;
