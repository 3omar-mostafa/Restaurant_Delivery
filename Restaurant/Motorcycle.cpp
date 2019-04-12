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
	startTime=-1;
	status=IDLE;
}

bool Motorcycle::operator>(Motorcycle & right)
{
	return speed > right.speed;
}


Motorcycle::~Motorcycle()
{
}

void Motorcycle::setStatus(STATUS s)
{
	status=s;
}

void Motorcycle::setStartTime(int time)
	{
		startTime=time;
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

	int Motorcycle::getStartTime() const
	{
       return startTime;
	}