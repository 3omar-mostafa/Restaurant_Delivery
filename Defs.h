#ifndef __DEFS_H_
#define __DEFS_H_

#include <cstdlib>
#include <time.h>
#include <iostream>
#include <fstream>
#include <cmath>

using namespace std;

//The four regions: A, B, C, and D
enum REGION
{
	A_REGION,
	B_REGION,
	C_REGION,
	D_REGION,
	REGION_COUNT //Total number of regions
};

//Types of Orders
enum ORDER_TYPE
{
	TYPE_NORMAL, //normal order
	TYPE_FROZEN, //frozen
	TYPE_VIP,	//VIP
	TYPE_COUNT   //Number of types

};

//Status of Motorcycles
enum STATUS
{
	IDLE,   //idle ==> waiting to be assigned at order
	SERVICE //is-service ==> is delivering at order but hasn't returned yet.
};

//Program Interface Mode
enum PROGRAM_MODE
{
	MODE_INTERACTIVE, //interactive mode
	MODE_STEP,		  //Step-by-step mode
	MODE_SILENT,	  //Silent mode
	MODE_RAMADAN,	 //Mode for Ramadan holy month
	MODE_COUNT		  //number of possible modes
};

// Types of Motorcycles
enum MOTOR_TYPE
{
	MOTOR_FAST,
	MOTOR_FROZEN,
	MOTOR_NORMAL,
	MOTOR_COUNT
};

template <typename T>
//Comparator function which has a specialization to deal with orders.
bool isGreaterThan(T left, T right);

#define MaxPossibleOrdCnt 999 //arbitrary value

#endif