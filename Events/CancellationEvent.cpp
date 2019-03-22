#include "CancellationEvent.h"
#include "..\Restaurant\Restaurant.h"



CancellationEvent::CancellationEvent(int eTime, int ordID) :Event(eTime, ordID)
{
}

/*CancellationEvent::CancellationEvent():Event(-1, -1)
{
}*/


CancellationEvent::~CancellationEvent()
{
}

void CancellationEvent::readData(ifstream & inFile)
{
	inFile >> eventTime >> orderID;
}

void CancellationEvent::execute(Restaurant * pRest)
{
	pRest->cancel(orderID);
}
