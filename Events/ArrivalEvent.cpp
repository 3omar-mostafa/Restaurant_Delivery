#include "ArrivalEvent.h"
#include "..\Restaurant\Restaurant.h"


/*ArrivalEvent::ArrivalEvent(int eTime, int oID, ORDER_TYPE oType, REGION reg):Event(eTime, oID)
{
	orderType = oType;
	orderRegion = reg;
}*/

ArrivalEvent::ArrivalEvent(int eTime, int oID, ORDER_TYPE oType, REGION reg, double oMoney) :Event(eTime, oID)
{
	orderType = oType;
	orderRegion = reg;
	orderMoney = oMoney;
}



ArrivalEvent::ArrivalEvent() :Event(-1, -1)
{
}

void ArrivalEvent::execute(Restaurant* pRest)
{
	//This function should create an order and and fills its data 
	// Then adds it to normal, frozen, or VIP order lists that you will create in phase1

	///For the sake of demo, this function will just create an order and add it to DemoQueue
	///Remove the next code lines in phase 1&2
	Order* pOrd = new Order(orderID, orderType, orderRegion);
	pRest->orderOfID(orderID) = pOrd;

	pRest->AddtoDemoQueue(pOrd);
}

void ArrivalEvent::readData(ifstream & inFile)
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
		orderType = TYPE_NORMAL;
		break;
	}
	orderRegion = REGION('A' - charOrderRegion);
}
