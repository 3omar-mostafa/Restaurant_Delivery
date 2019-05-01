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
	Queue<Event *> eventsQueue; //Queue of all events that will be loaded from file

	int autoPromotionLimit;

	//Orders:
	//	3rd idea:
	//		A PriorityQueue for VIP orders.
	//		A separate Data Structure(queue?)-->(MixedList) for Normal orders, which handles Auto and Manual Promotion, and cancellation.
	//		A Queue for Frozen orders.
	//		Separate Queues for each region.
	//		Orders of each individual Timestep are added to each Queue in that specific Timestep,
	//		only sorting the orders of that timestep with respect to each other.
	PriorityQueue<Order *> vipQueue[REGION_COUNT];
	LinkedList<Order *> normalQueue[REGION_COUNT];
	Queue<Order *> frozenQueue[REGION_COUNT];

	PriorityQueue<Order *> totalQueue;

	//Motorcycles:
	//	1st idea:
	//		3 PriorityQueues for each type, fastest Motorcycles have the highest priority.
	//		4 Regions, each region has its own PriorityQueues.
	PriorityQueue<Motorcycle *>
		vipMotorQueue[REGION_COUNT],
		normalMotorQueue[REGION_COUNT],
		frozenMotorQueue[REGION_COUNT];

	LinkedList<Motorcycle*>
		inServiceMotorcycles[REGION_COUNT];

	//Array of Order* to handle cancellations.
	Order *orderIdArray[MaxPossibleOrdCnt];

  public:
	Restaurant();
	~Restaurant();
	void runSimulation();

	// Event functions:
	void addEvent(Event *pE);		  //adds a new event to the queue of events
	void executeEvents(int TimeStep); //executes all events at current timestep

	void loadFromFile(string fileName);
	void writeToFile(string filename);

	
	void Operate(int mode);
	void interactiveMode();
	void stepByStepMode();
	void silentMode();

	// Queue functions:
	void showActiveOrders();
	void addToActiveQueue(Order *pOrd); //Adds the order to its proper Queue
	bool finished();

	// Order functions:
	Order *&orderOfID(int i);

	// Promotion functions:
	bool promote(int id, int extraMoney);
	bool autoPromoteRegion(int currentTimeStep, REGION reg); //Handles auto-promotion of Normal orders to VIP orders
	void autoPromoteAll(int currentTimeStep);

	// Cancellation functions:
	bool cancel(int id);

	// Motorcycle functions:
	void assignOrderToMotorcycle(int currentTimestep, Order *pOrd, Motorcycle *pMotor);
	void assignMotorcycles(int currentTimestep);
	void returnMotorcycles(int currentTimestep);

	// GUI functions:
	void displayRegionsData();
};

#endif