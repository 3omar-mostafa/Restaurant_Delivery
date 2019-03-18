#include "Order.h"

void Order::setPriority()
{
}

Order::Order(int id, ORDER_TYPE r_Type, REGION r_region)
{
	ID = (id>0&&id<1000)?id:0;	//1<ID<999
	type = r_Type;
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
	distance = d>0?d:0;
}

int Order::GetDistance() const
{
	return distance;
}
