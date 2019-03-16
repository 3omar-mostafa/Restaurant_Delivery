#include "ArrivalEvent.h"
#include "..\Rest\Restaurant.h"


ArrivalEvent::ArrivalEvent(int eTime, int oID, ORDER_TYPE oType, REGION reg):Event(eTime, oID)
{
	orderType = oType;
	orderRegion = reg;
}

void ArrivalEvent::execute(Restaurant* pRest)
{
	//This function should create and order and and fills its data 
	// Then adds it to normal, frozen, or VIP order lists that you will create in phase1

	
	
	///For the sake of demo, this function will just create an order and add it to DemoQueue
	///Remove the next code lines in phase 1&2
	Order* pOrd = new Order(orderID,orderType,orderRegion);
	pRest->AddtoDemoQueue(pOrd);
}
