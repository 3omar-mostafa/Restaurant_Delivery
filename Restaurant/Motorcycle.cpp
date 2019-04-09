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
}

bool Motorcycle::operator>(Motorcycle & right)
{
	return speed > right.speed;
}


Motorcycle::~Motorcycle()
{
}
