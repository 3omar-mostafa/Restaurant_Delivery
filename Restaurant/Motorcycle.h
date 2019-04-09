#ifndef __MOTORCYCLE_H_
#define __MOTORCYCLE_H_


#include "..\Defs.h"

#pragma once
class Motorcycle	
{
	int ID;
	MOTOR_TYPE type;	//for each order type there is a corresponding motorcycle type 
	int speed;		//meters it can move in one clock tick (in one timestep)
	REGION	region;	//region of the motorcycle
	STATUS	status;	//idle or in-service

public:
	Motorcycle();
	Motorcycle(int inID, MOTOR_TYPE inType, int inSpeed, REGION inRegion);
	bool operator>(Motorcycle& right);
	virtual ~Motorcycle();
};

#endif