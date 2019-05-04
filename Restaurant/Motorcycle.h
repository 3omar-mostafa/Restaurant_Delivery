#ifndef __MOTORCYCLE_H_
#define __MOTORCYCLE_H_


#include "..\Defs.h"

#pragma once
class Motorcycle	
{
	int ID, orderID;
	MOTOR_TYPE type;	//for each order type there is a corresponding motorcycle type 
	int speed;			//meters it can move in one clock tick (in one timestep)
	REGION	region;		//region of the motorcycle
	STATUS	status;		//idle or in-service
	int finishTime;		//The time which the motorcycle finishes service
	int hp;
	bool isDamaged;

	//Useless
	int startTime; 		//the time which the motorcycle starts service


  public:
	Motorcycle();
	Motorcycle(int inID, MOTOR_TYPE inType, int inSpeed, REGION inRegion);
	bool operator>(Motorcycle& right) const;
	virtual ~Motorcycle();
	
	MOTOR_TYPE getType() const;	
	REGION getRegion() const;	
	int getSpeed() const;

	void setOrderID(int oID);
	int getOrderID() const;

	int getID() const;

	void setHP(int Hp);
	int getHP() const;
	bool getState()const;

	void setStatus(STATUS s);
	STATUS getStatus() const;
	
	void setFinishTime(int startTime, int distance);
	int getFinishTime() const;
	
	//Useless
	void setStartTime(int time);
	int getStartTime() const;

};

#endif