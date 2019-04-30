#include "Motorcycle.h"

Motorcycle::Motorcycle()
{
}

Motorcycle::Motorcycle(int inID, MOTOR_TYPE inType, int inSpeed, REGION inRegion)
{
	ID = inID;
	orderID = -1;
	type = inType;
	speed = inSpeed;
	region = inRegion;
	finishTime = -1;
	setStatus(IDLE);

	startTime = -1;
}

Motorcycle::~Motorcycle()
{
}


bool Motorcycle::operator>(Motorcycle &right)
{
	return speed > right.speed;
}

void Motorcycle::setStatus(STATUS s)
{
	status = s;
	if (status == IDLE)
		setOrderID(-1);
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
	this->startTime = startTime;
	finishTime = startTime + ceil((distance * 2.0) / speed);
}

int Motorcycle::getFinishTime() const
{
	return finishTime;
}

void Motorcycle::setOrderID(int oID)
{
	orderID = oID;
}

int Motorcycle::getOrderID() const
{
	return orderID;
}

//Useless
void Motorcycle::setStartTime(int time)
{
	startTime = time;
}

int Motorcycle::getStartTime() const
{
	return startTime;
}
