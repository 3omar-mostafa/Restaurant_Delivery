#include "CancellationEvent.h"
#include "..\Restaurant\Restaurant.h"

CancellationEvent::CancellationEvent(int eTime, int ordID) : Event(eTime, ordID)
{
}

void CancellationEvent::readData(ifstream &inFile)
{
	inFile >> eventTime >> orderID;
}

void CancellationEvent::execute(Restaurant *pRest)
{
	pRest->cancel(orderID,pRest->getTimeStep());
}
