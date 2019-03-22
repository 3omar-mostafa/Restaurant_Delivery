#ifndef __DEFS_H_
#define __DEFS_H_

#include <cstdlib>
#include <time.h>
#include <iostream>
#include <fstream>

using namespace std;

//The four regions: A, B, C , and D
enum REGION {	
	A_REGION,
	B_REGION,
	C_REGION,
	D_REGION,
	REGION_COUNT		//Total number of regions
};


enum ORDER_TYPE	//types of orders
{
	TYPE_NORMAL,	//normal order
	TYPE_FROZEN,	//frozen
	TYPE_VIP,	//VIP
	TYPE_COUNT	//Number of types

};

enum STATUS	//status of motorcycles
{
	IDLE,	//idle ==> waiting to be assigned at order
	SERVICE	//is-service ==> is delivering at order but hasn't returned yet.
};


enum PROGRAM_MODE	//mode of the program interface
{
	MODE_INTERACTIVE,	//interactive mode
	MODE_STEP,	//Step-by-step mode
	MODE_SILENT,	//Silent mode
	MODE_DEMO,	//Demo mode (for introductory phase only, should be removed in phases 1&2)
	MODE_COUNT	//number of possible modes
};


#define MaxPossibleOrdCnt 999	//arbitrary value





#endif