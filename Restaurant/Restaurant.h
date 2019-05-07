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
	GUI *pGUI;
	Queue<Event *> eventsQueue; //Queue of all events that will be loaded from file

	int autoPromotionLimit;
	int timeStep;
	//Orders:
	//	3rd idea:
	//		A PriorityQueue for VIP orders.
	//		A LinkedList for Normal orders, which handles Auto and Manual Promotion, and Cancellation.
	//		A Queue for Frozen orders.
	//		Separate Queues for each region.
	//		A PriorityQueue for Served Orders.
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
	//		A PriorityQueue for Damaged Motorcycles.
	//		A LinkedList for Motorcycles that are in service.
	PriorityQueue<Motorcycle *>
		vipMotorQueue[REGION_COUNT],
		normalMotorQueue[REGION_COUNT],
		frozenMotorQueue[REGION_COUNT],
		damagedMotorQueue[REGION_COUNT];

	LinkedList<Motorcycle *>
		inServiceMotorcycles[REGION_COUNT];

	int totalOrdersServed[REGION_COUNT][TYPE_COUNT];
	string assignedMotorcyclesLastTimestep[REGION_COUNT];

	//Array of Order* to handle cancellations.
	Order *orderIdArray[MaxPossibleOrdCnt];

public:
	Restaurant();
	~Restaurant();


	int getTimeStep() const;
	// Main Program functions:

	void runSimulation();
	void Operate(PROGRAM_MODE mode);
	void Ramadan(int currentTimestep);

	// Event functions:

	void addEvent(Event *pE);
	void executeEvents(int TimeStep);

	// File functions:

	void loadFromFile(string fileName);
	void writeToFile(string filename);

	// Order Queue functions:

	void showActiveOrders();
	void addToActiveQueue(Order *pOrd);
	bool finished() const;

	Order *&orderOfID(int i);

	void displayRegionsData();

	// Order functions:

	bool promote(int id, int extraMoney);
	bool autoPromoteRegion(int currentTimeStep, REGION reg);
	void autoPromoteAll(int currentTimeStep);

	bool cancel(int id,int time);

	// Motorcycle functions:

	void assignOrderToMotorcycle(int currentTimestep, Order *pOrd, Motorcycle *pMotor) const;
	void assignMotorcycles(int currentTimestep);
	void returnMotorcycles(int currentTimestep);
	string assignedMotorcyclesData(Motorcycle *, Order *) const;
};

#endif