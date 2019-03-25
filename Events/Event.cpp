#include "Event.h"


Event::Event(int eTime, int ordID)
{
	eventTime = eTime;
	orderID	= ordID;
}


int Event::getEventTime() const
{
	return eventTime;
}
int Event::getOrderID() const
{
	return orderID;
}


Event::~Event()
{

}

