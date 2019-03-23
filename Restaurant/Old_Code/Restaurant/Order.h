#ifndef __ORDER_H_
#define __ORDER_H_

#include "..\Defs.h"

class Order
{

protected:
	int ID;         //Each order has a unique ID (from 1 --> 999 )
	ORDER_TYPE type;		//order type: Normal, Frozen, VIP
	REGION region;  //Region of this order
	int distance;	//The distance (in meters) between the order location and the restaurant 
	                
	double totalMoney;	//Total order money

	int arrivalTime, serviceTime, finishTime;	//arrival, service start, and finish times
	
	
	//
	// TODO: Add More Data Members As Needed
	//

public:
	Order(int ID, ORDER_TYPE r_Type, REGION r_region);
	virtual ~Order();

	int GetID();

	int GetType() const;
	REGION GetRegion() const;

	void SetDistance(int d);
	int GetDistance() const;

	//
	// TODO: Add More Member Functions As Needed
	//

};

#endif