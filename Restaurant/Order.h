#ifndef __ORDER_H_
#define __ORDER_H_

#include "..\Defs.h"

class Order
{
	void setPriority();

protected:
	int ID;         //Each order has a unique ID (from 1 --> 999)
	ORDER_TYPE type;		//order type: Normal, Frozen, VIP
	REGION region;  //Region of this order
	int distance;	//The distance (in meters) between the order location and the restaurant 
	                
	double totalMoney;	//Total order money

	float priority;		//Used to sort the elements in the PriorityQueue

	int arrivalTime, serviceTime, finishTime;	//arrival, service start, and finish times
	
	
	//
	// TODO: Add More Data Members As Needed
	//

public:
	Order(int ID, ORDER_TYPE r_Type, REGION r_region);
	virtual ~Order();

	int GetID();
	bool operator==(int right);
	// Used to sort the elements in the PriorityQueue
	bool operator>(Order& right);

	int GetType() const;
	REGION GetRegion() const;

	void SetDistance(int d);
	int GetDistance() const;
		
	//
	// TODO: Add More Member Functions As Needed
	//


	void setType(ORDER_TYPE newType);
	bool promote(int extraMoney = 0);
	int getArrivalTime() const;

};

// Adding all Order* into an array, to be called by their unique IDs.
Order* orderIdArray[MaxPossibleOrdCnt];


#endif