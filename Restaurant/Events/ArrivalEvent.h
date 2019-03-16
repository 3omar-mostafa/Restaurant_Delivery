#ifndef __ARRIVAL_EVENT_H_
#define __ARRIVAL_EVENT_H_

#include "Event.h"


//class for the arrival event
class ArrivalEvent: public Event
{
	//info about the order ralted to arrival event
	int orderDistance;	//order distance
	ORDER_TYPE orderType;		//order type: Normal, Frozen, VIP
	REGION orderRegion;  //Region of this order	                
	double orderMoney;	//Total order money
public:
	ArrivalEvent(int eTime, int oID, ORDER_TYPE oType, REGION reg);
	//Add more constructors if needed
	
	void execute(Restaurant *pRest) override;	//override execute function

};

#endif