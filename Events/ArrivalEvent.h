#ifndef __ARRIVAL_EVENT_H_
#define __ARRIVAL_EVENT_H_

#include "Event.h"

//class for the arrival event
class ArrivalEvent : public Event
{
	//info about the order related to arrival event
	int orderDistance;	//order distance
	ORDER_TYPE orderType; //order type: Normal, Frozen, VIP
	REGION orderRegion;   //Region of this order
	double orderMoney;	//Total order money
public:
	ArrivalEvent(int eTime, ORDER_TYPE oType, int oID, int oDistance, double oMoney, REGION oRegion);
	ArrivalEvent();

	void readData(ifstream &inFile) override;
	void execute(Restaurant *pRest) override;
};

#endif