#include "Motorcycle.h"

Motorcycle::Motorcycle()
{
}

Motorcycle::Motorcycle(int inID, MOTOR_TYPE inType, int inSpeed, REGION inRegion)
{
	ID = inID;
	type = inType;
	speed = inSpeed;
	region = inRegion;
	finishTime = -1;
	status = IDLE;
	
	//startTime = -1;
}

bool Motorcycle::operator>(Motorcycle &right)
{
	return speed > right.speed;
}

Motorcycle::~Motorcycle()
{
}

void Motorcycle::setStatus(STATUS s)
{
	status = s;
}


MOTOR_TYPE Motorcycle::getType() const
{
	return type;
}

int Motorcycle::getSpeed() const
{
	return speed;
}

REGION Motorcycle::getRegion() const
{
	return region;
}

STATUS Motorcycle::getStatus() const
{
	return status;
}


void Motorcycle::setFinishTime(int startTime, int distance)
{
	finishTime = startTime + (distance * 2) / speed;
}

int Motorcycle::getFinishTime() const
{
	return finishTime;
}


//Useless
/*void Motorcycle::setStartTime(int time)
{
	startTime = time;
}
int Motorcycle::getStartTime() const
{
	return startTime;
}*/
