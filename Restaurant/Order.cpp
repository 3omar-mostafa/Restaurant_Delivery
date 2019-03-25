#include "Order.h"

void Order::setPriority()
{
	priority = (type == TYPE_VIP) ? (400.0 / arrivalTime + 100.0 / distance + 0.1 * totalMoney) : -1;
}

void Order::setType(ORDER_TYPE newType)
{
	type = newType;
}

bool Order::promote(int extraMoney)
{
	if (type == TYPE_VIP || type == TYPE_FROZEN)
		return false;

	type = TYPE_VIP;
	totalMoney += extraMoney;
	setPriority();
	return true;
}

Order::Order(int r_Time, ORDER_TYPE r_Type, int r_ID, int r_Distance, int r_Money, REGION r_region)
{
	arrivalTime = r_Time;
	type = r_Type;
	ID = (r_ID > 0 && r_ID < 1000) ? r_ID : 0;	//1<ID<999
	distance = r_Distance;
	totalMoney = r_Money;
	region = r_region;
	setPriority();
}

Order::~Order()
{
}

int Order::GetID()
{
	return ID;
}

bool Order::operator==(int id)
{
	return ID == id;
}


bool Order::operator>(Order & right)
{
	return priority > right.priority;
}

int Order::GetType() const
{
	return type;
}

REGION Order::GetRegion() const
{
	return region;
}

void Order::SetDistance(int d)
{
	distance = d > 0 ? d : 0;
}

int Order::GetDistance() const
{
	return distance;
}

int Order::getArrivalTime() const
{
	return arrivalTime;
}
