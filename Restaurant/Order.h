#ifndef __ORDER_H_
#define __ORDER_H_

#include "..\Defs.h"

class Order
{
  protected:
	int ID;			 //Each order has a unique ID (from 1 --> 999)
	ORDER_TYPE type; //order type: Normal, Frozen, VIP
	REGION region;   //Region of this order
	int distance;	//The distance (in meters) between the order location and the restaurant
	bool toBeDeleted;

	double totalMoney; //Total order money

	float priority; //Used to sort the elements in the PriorityQueue

	int arrivalTime, serviceTime, finishTime, waitTime; //arrival, service, finish, and wait times

	//
	// TODO: Add More Data Members As Needed
	//

  public:
	Order(int r_Time, ORDER_TYPE r_Type, int r_ID, int r_Distance, int r_Money, REGION r_region);
	virtual ~Order();

	int getID() const;
	bool operator ==(int id) const;

	// Used to sort the elements in the PriorityQueue
	void setPriority(int mode = 0);
	bool operator>(Order &right) const;

	ORDER_TYPE GetType() const;
	REGION GetRegion() const;

	void setDistance(int d);
	int getDistance() const;
	bool isDeleted() const;
	void deleteIt(bool del);

	//
	// TODO: Add More Member Functions As Needed
	//

	void setTimes(int startTime, int speed);

	int getFinishTime() const;
	int getWaitTime() const;
	int getArrivalTime() const;
	int getServiceTime() const;

	void writeData(ofstream &outFile) const;

	void setType(ORDER_TYPE newType);
	bool promote(int extraMoney = 0);
};

#endif