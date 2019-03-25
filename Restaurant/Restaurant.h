#ifndef __RESTAURANT_H_
#define __RESTAURANT_H_

#include "..\Defs.h"
#include "..\CMUgraphicsLib\CMUgraphics.h"
#include "..\GUI\GUI.h"
#include "..\Generic_DS\PriorityQueue.h"
#include "..\Generic_DS\Queue.h"
#include "..\Generic_DS\LinkedList.h"
#include "..\Events\Event.h"

#include "Motorcycle.h"
#include "Order.h"


// it is the maestro of the project
class Restaurant  
{	
private:
	GUI *pGUI;
	Queue<Event*> eventsQueue;	//Queue of all events that will be loaded from file

	
	/// ==> 
	//	DEMO-related members. Should be removed in phases 1&2
	Queue<Order*> DEMO_Queue;	//Important: This is just for demo
	/// ==>
	
	//
	// TODO: Add More Data Members As Needed
	//
	int autoPromotionLimit;

	//Orders:
	//	3rd idea: 
	//		A PriorityQueue for VIP orders.
	//		A separate Data Structure(queue?)-->(MixedList) for Normal orders, which handles Auto and Manual Promotion, and cancellation.
	//		A Queue for Frozen orders.
	//		Separate Queues for each region.
	//		Orders of each individual Timestep are added to each Queue in that specific Timestep,
	//		only sorting the orders of that timestep with respect to each other.	
	PriorityQueue<Order*> vipQueue[REGION_COUNT];
	LinkedList<Order*> normalQueue[REGION_COUNT];
	Queue<Order*> frozenQueue[REGION_COUNT];

	//Motorcycles:
	//	1st idea:
	//		3 PriorityQueues for each type, fastest Motorcycles have the highest priority.
	//		4 Regions, each region has its own PriorityQueues.
	PriorityQueue<Motorcycle*>
		vipMotorQueue[REGION_COUNT],
		normalMotorQueue[REGION_COUNT],
		frozenMotorQueue[REGION_COUNT];

	//Array of Order* to handle cancellations.
	Order* orderIdArray[MaxPossibleOrdCnt];

public:
	
	Restaurant();
	~Restaurant();
	void addEvent(Event* pE);	//adds a new event to the queue of events
	void executeEvents(int TimeStep);	//executes all events at current timestep
	void runSimulation();

	/// ==> 
	///  DEMO-related functions. Should be removed in phases 1&2
	void Just_A_Demo();	//just to show a demo and should be removed in phase1 1 & 2
	void AddtoDemoQueue(Order* po);	//adds an order to the demo queue
	Order* getDemoOrder();			//return the front order from demo queue
	/// ==> 

	//
	// TODO: Add More Member Functions As Needed
	//
	void addToActiveQueue(Order* pOrd);	//Adds the order to its proper Queue
	void loadFromFile(string fileName);
	Order*& orderOfID(int i);
	void interactiveMode();

	// Promotion functions:
	bool autoPromoteRegion(int currentTimeStep, REGION reg);	//Handles auto-promotion of Normal orders to VIP orders
	void autoPromoteAll(int currentTimeStep);

	// Cancellation functions:
	bool cancel(int id);

};

#endif