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
	int startTime; //the time which the motorcycle starts service 
public:
	Motorcycle();
	Motorcycle(int inID, MOTOR_TYPE inType, int inSpeed, REGION inRegion);
	bool operator>(Motorcycle& right);
	virtual ~Motorcycle();
	void setStatus(STATUS s);
	void setStartTime(int time);
	MOTOR_TYPE getType() const;
	int getSpeed() const;
	REGION getRegion() const;
	STATUS getStatus() const;
	int getStartTime() const;
	
};

#endif