#include "Restaurant.h"
#include "..\Events\ArrivalEvent.h"
#include "..\Events\CancellationEvent.h"


template <typename T>
bool isGreaterThan(T left, T right)
{
	return left > right;
}

template <typename T>
//Compares the references pointed to by the pointers.
bool isGreaterThan(T* left, T* right)
{
	return *left > *right;
}


Restaurant::Restaurant()
{
	pGUI = NULL;
}

void Restaurant::runSimulation()
{
	pGUI = new GUI;
	PROGRAM_MODE mode = pGUI->getGUIMode();

	switch (mode)	//Add a function for each mode in next phases
	{
	case MODE_INTERACTIVE:
		interactiveMode();
		break;

	case MODE_STEP:
		break;

	case MODE_SILENT:
		break;

	case MODE_DEMO:
		//Just_A_Demo();
		break;
	};

}



//////////////////////////////////  Event handling functions   /////////////////////////////

void Restaurant::addEvent(Event* pE)	//adds a new event to the queue of events
{
	eventsQueue.enqueue(pE);
}

//Executes ALL events that should take place at current timestep
void Restaurant::executeEvents(int CurrentTimeStep)
{
	Event *pE;
	while (eventsQueue.peekFront(pE))	//as long as there are more events
	{
		if (pE->getEventTime() > CurrentTimeStep)	//no more events at current time
			return;

		pE->execute(this);
		eventsQueue.dequeue(pE);	//remove event from the queue
		delete pE;		//deallocate event object from memory
	}

}


Restaurant::~Restaurant()
{
	delete pGUI;
}

////////////////////////////////////////////////////////////////////////////////


//Simple simulator function for Phase 1:
void Restaurant::interactiveMode()
{
	loadFromFile("input.txt");
	int currentTimeStep = 1;
	while (!eventsQueue.isEmpty() || !finished())
	{
		//print current timestep
		/*char timestep[10];
		itoa(currentTimeStep, timestep, 10);
		pGUI->PrintMessage(timestep);*/

		//Execute all events at current time step
		executeEvents(currentTimeStep);

		//Show all active orders in each region
		showActiveOrders();
		pGUI->UpdateInterface();

		//Display region info on the status bar
		string regionsData = to_string(currentTimeStep) + " --> ";
		for (int reg = 0; reg < REGION_COUNT; reg++)
		{
			int noActiveOrdersOf[TYPE_COUNT] = { 0, 0, 0 };
			int noAvailableMotor = 0;
			noActiveOrdersOf[TYPE_NORMAL] += normalQueue[reg].getLength();
			noActiveOrdersOf[TYPE_FROZEN] += frozenQueue[reg].getLength();
			noActiveOrdersOf[TYPE_VIP] += vipQueue[reg].getLength();
			noAvailableMotor += normalMotorQueue[reg].getLength() + frozenMotorQueue[reg].getLength() + vipMotorQueue[reg].getLength();
			regionsData += "(";
			regionsData += char('A' + reg);
			regionsData += ") " + to_string(noActiveOrdersOf[TYPE_NORMAL]) + "N ";
			regionsData += to_string(noActiveOrdersOf[TYPE_FROZEN]) + "F ";
			regionsData += to_string(noActiveOrdersOf[TYPE_VIP]) + "V ";
			regionsData += to_string(noAvailableMotor) + "Mc  ";
		}
		pGUI->PrintMessage(regionsData);

		//Delete the top order in each queue
		for (int reg = 0; reg < REGION_COUNT; reg++)
		{
			Order* pOrd = 0;
			normalQueue[reg].pop(pOrd);
			frozenQueue[reg].dequeue(pOrd);
			vipQueue[reg].dequeue(pOrd);
		}

		//Update the interface again, increase the timestep while resetting the list of objects drawn on the screen
		pGUI->UpdateInterface();
		pGUI->waitForClick();
		currentTimeStep++;
		pGUI->ResetDrawingList();
	}

	pGUI->UpdateInterface();
	pGUI->PrintMessage("Simulation over.");
	pGUI->waitForClick();
}

void Restaurant::loadFromFile(string fileName)
{
	//Reading from the input file:
	ifstream inFile;
	inFile.open(fileName);

	//Colecting Motorcycle data (((((((((((((((pre-bonus))))))))))))))):	
	int normalMotorSpeed, frozenMotorSpeed, vipMotorSpeed;
	inFile >> normalMotorSpeed >> frozenMotorSpeed >> vipMotorSpeed;

	int normalMotorCount[REGION_COUNT], frozenMotorCount[REGION_COUNT], vipMotorCount[REGION_COUNT];
	for (int i = 0; i < REGION_COUNT; i++)
	{
		inFile >> normalMotorCount[i] >> frozenMotorCount[i] >> vipMotorCount[i];
		//Enqueuing Motorcycles of each type in each region:
		for (int j = 0; j < normalMotorCount[i]; j++)
		{
			Motorcycle* normalMotor = new Motorcycle(0, TYPE_NORMAL, normalMotorSpeed, REGION(i));
			normalMotorQueue[i].enqueue(normalMotor);
		}

		for (int j = 0; j < frozenMotorCount[i]; j++)
		{
			Motorcycle* frozenMotor = new Motorcycle(0, TYPE_FROZEN, frozenMotorSpeed, REGION(i));
			frozenMotorQueue[i].enqueue(frozenMotor);
		}

		for (int j = 0; j < vipMotorCount[i]; j++)
		{
			Motorcycle* vipMotor = new Motorcycle(0, TYPE_VIP, vipMotorSpeed, REGION(i));
			vipMotorQueue[i].enqueue(vipMotor);
		}
	}

	//Time spent for Auto-Promotion:
	inFile >> autoPromotionLimit;

	//Logging events into the queue:
	int noOfEvents;
	inFile >> noOfEvents;

	for (int i = 0; i < noOfEvents; i++)
	{
		char eventType;
		inFile >> eventType;
		Event* toBeAdded;
		switch (eventType)
		{
		case 'R':
			toBeAdded = new ArrivalEvent;
			break;

		case 'X':
			toBeAdded = new CancellationEvent;
			break;

		case 'P':
			//toBeAdded = new PromotionEvent;
			break;
		}
		toBeAdded->readData(inFile);
		addEvent(toBeAdded);
	}
	inFile.close();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Restaurant::addToActiveQueue(Order * pOrd)
{
	REGION reg = pOrd->GetRegion();
	switch (pOrd->GetType())
	{
	case TYPE_NORMAL:
		normalQueue[reg].append(pOrd);
		break;

	case TYPE_FROZEN:
		frozenQueue[reg].enqueue(pOrd);
		break;

	case TYPE_VIP:
		vipQueue[reg].enqueue(pOrd);
		break;
	}
}


//	Shows all orders currently in queues.
//	For each region, pops the order from its respectful queue, shows it on screen, and then appends it to the end of the list.
//	PriorityQueue requires special treatment.

void Restaurant::showActiveOrders()
{
	for (int reg = 0; reg < REGION_COUNT; reg++)
	{
		Order* pOrd = 0;

		int nQLength = normalQueue[reg].getLength();
		for (int i = 0; i < nQLength; i++)
		{
			if (normalQueue[reg].pop(pOrd))
			{
				pGUI->AddOrderForDrawing(pOrd);
				normalQueue[reg].append(pOrd);
			}
		}

		int fQLength = frozenQueue[reg].getLength();
		for (int i = 0; i < fQLength; i++)
		{
			if (frozenQueue[reg].dequeue(pOrd))
			{
				pGUI->AddOrderForDrawing(pOrd);
				frozenQueue[reg].enqueue(pOrd);
			}
		}

		PriorityQueue<Order*> tempVIPQueue = PriorityQueue<Order*>(vipQueue[reg]);
		while (tempVIPQueue.dequeue(pOrd))
			pGUI->AddOrderForDrawing(pOrd);
	}
}

bool Restaurant::finished()
{
	for (int reg = 0; reg < REGION_COUNT; reg++)
		if (!(normalQueue[reg].isEmpty() && frozenQueue[reg].isEmpty() && vipQueue[reg].isEmpty()))
			return false;
	return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

Order *& Restaurant::orderOfID(int i)
{
	return orderIdArray[i];
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool Restaurant::autoPromoteRegion(int currentTimeStep, REGION reg)
{
	Order* toBePromoted;
	if (!normalQueue[reg].peekFront(toBePromoted))
		return false;
	if (currentTimeStep - toBePromoted->getArrivalTime() >= autoPromotionLimit)
	{
		normalQueue[reg].pop(toBePromoted);
		toBePromoted->promote();
		vipQueue[reg].enqueue(toBePromoted);
		return true;
	}
	return false;
}

void Restaurant::autoPromoteAll(int currentTimeStep)
{
	for (int i = 0; i < REGION_COUNT; i++)
		while (autoPromoteRegion(currentTimeStep, REGION(i)));
}

bool Restaurant::cancel(int id)
{
	Order* cancelledOrder = orderIdArray[id];
	for (int i = 0; i < REGION_COUNT; i++)
	{
		if (normalQueue[i].remove(cancelledOrder))
			return true;
	}
	return false;
}


/// ==> 
///  DEMO-related functions. Should be removed in phases 1&2

//This is just a demo function for project introductory phase
//It should be removed starting phase 1
/*

void Restaurant::Just_A_Demo()
{
	
		//
		// THIS IS JUST A DEMO FUNCTION
		// IT SHOULD BE REMOVED IN PHASE 1 AND PHASE 2

		int EventCnt;
		Order* pOrd;
		Event* pEv;
		srand(time(NULL));

		pGUI->PrintMessage("Just a Demo. Enter EVENTS Count(next phases should read I/P filename):");
		EventCnt = atoi(pGUI->GetString().c_str());	//get user input as a string then convert to integer

		pGUI->UpdateInterface();

		pGUI->PrintMessage("Generating orders randomly... In next phases, orders should be loaded from a file");

		int EvTime = 0;

		//Create Random events
		//All generated event will be "ArrivalEvents" for the demo
		for (int i = 0; i < EventCnt; i++)
		{
			int O_id = i + 1;

			//Rendomize order type
			int OType;
			if (i < EventCnt*0.2)	//let 1st 20% of orders be VIP (just for sake of demo)
				OType = TYPE_VIP;
			else if (i < EventCnt*0.5)
				OType = TYPE_FROZEN;	//let next 30% be Frozen
			else
				OType = TYPE_NORMAL;	//let the rest be normal


			int reg = rand() % REGION_COUNT;	//randomize region


			//Randomize event time
			EvTime += rand() % 4;
			pEv = new ArrivalEvent(EvTime, O_id, (ORDER_TYPE)OType, (REGION)reg);
			addEvent(pEv);

		}

		int CurrentTimeStep = 1;
		//as long as events queue is not empty yet
		while (!eventsQueue.isEmpty())
		{
			//print current timestep
			char timestep[10];
			itoa(CurrentTimeStep, timestep, 10);
			pGUI->PrintMessage(timestep);


			executeEvents(CurrentTimeStep);	//execute all events at current time step
			//The above line may add new orders to the DEMO_Queue

			//Let's draw all arrived orders by passing them to the GUI to draw

			while (DEMO_Queue.dequeue(pOrd))
			{
				pGUI->AddOrderForDrawing(pOrd);
				pGUI->UpdateInterface();
			}
			Sleep(1000);
			CurrentTimeStep++;	//advance timestep
		}


		pGUI->PrintMessage("generation done, click to END program");
		pGUI->waitForClick();

		
}


void Restaurant::AddtoDemoQueue(Order *pOrd)
{
	DEMO_Queue.enqueue(pOrd);
}


Order* Restaurant::getDemoOrder()
{
	Order* pOrd;
	DEMO_Queue.dequeue(pOrd);
	return pOrd;

}
*/

/// ==> end of DEMO-related functions

////////////////
