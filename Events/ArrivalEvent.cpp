#include "ArrivalEvent.h"
#include "..\Restaurant\Restaurant.h"

ArrivalEvent::ArrivalEvent(int eTime, ORDER_TYPE oType, int oID, int oDistance, double oMoney, REGION oRegion) : Event(eTime, oID)
{
	orderType = oType;
	orderDistance = oDistance;
	orderMoney = oMoney;
	orderRegion = oRegion;
}

ArrivalEvent::ArrivalEvent() : Event(-1, -1)
{
}

void ArrivalEvent::readData(ifstream &inFile)
{
	char charOrderType, charOrderRegion;
	inFile >> eventTime >> charOrderType >> orderID >> orderDistance >> orderMoney >> charOrderRegion;
	switch (charOrderType)
	{
	case 'N':
		orderType = TYPE_NORMAL;
		break;

	case 'V':
		orderType = TYPE_VIP;
		break;

	case 'F':
		orderType = TYPE_FROZEN;
		break;
	}
	orderRegion = REGION(charOrderRegion - 'A');
}

void ArrivalEvent::execute(Restaurant *pRest)
{
	Order *pOrd = new Order(eventTime, orderType, orderID, orderDistance, orderMoney, orderRegion);
	pRest->orderOfID(orderID) = pOrd; //Add to order array

	pRest->addToActiveQueue(pOrd);
}
