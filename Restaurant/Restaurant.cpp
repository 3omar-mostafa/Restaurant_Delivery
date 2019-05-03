#include "Restaurant.h"
#include "..\Events\ArrivalEvent.h"
#include "..\Events\CancellationEvent.h"
#include "..\Events\PromotionEvent.h"

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
		stepByStepMode();
		break;

	case MODE_SILENT:
		silentMode();
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

void Restaurant::displayRegionsData()
{
	string regionsData[4] = {""};
	string regionsData2[4] = {""};

	for (int reg = 0; reg < REGION_COUNT; reg++)
	{
		int noActiveOrdersOf[TYPE_COUNT] = {0};
		int noAvailableMotor[TYPE_COUNT] = {0};

		noActiveOrdersOf[TYPE_NORMAL] += normalQueue[reg].getLength();
		noActiveOrdersOf[TYPE_FROZEN] += frozenQueue[reg].getLength();
		noActiveOrdersOf[TYPE_VIP] += vipQueue[reg].getLength();

		noAvailableMotor[TYPE_NORMAL] += normalMotorQueue[reg].getLength();
		noAvailableMotor[TYPE_FROZEN] += frozenMotorQueue[reg].getLength();
		noAvailableMotor[TYPE_VIP] += vipMotorQueue[reg].getLength();

		//regionsData[reg] += "(";
		//regionsData[reg] += char('A' + reg);
		//regionsData[reg] += ") ";

		regionsData[reg] += to_string(noActiveOrdersOf[TYPE_NORMAL]) + " Normal Orders             ";
		regionsData[reg] += to_string(noActiveOrdersOf[TYPE_FROZEN]) + " Frozen Orders             ";
		regionsData[reg] += to_string(noActiveOrdersOf[TYPE_VIP]) + " VIP Orders ";
																		//'   ||   '

		regionsData2[reg] += to_string(noAvailableMotor[TYPE_NORMAL]) + " Normal Motorcycles     ";
		regionsData2[reg] += to_string(noAvailableMotor[TYPE_FROZEN]) + " Frozen Motorcycles     ";
		regionsData2[reg] += to_string(noAvailableMotor[TYPE_VIP]) + " VIP Motorcycles";
	}
	pGUI->PrintRegions(regionsData, regionsData2);
}

void Restaurant::Operate(int mode)
{
	pGUI->PrintMessage("Enter the Input File Name (including .txt):");

	string inputFile = pGUI->GetString();
	loadFromFile(inputFile);

	int currentTimestep = 1;
	while (!eventsQueue.isEmpty() || !finished())
	{
		//Print current timestep
		pGUI->PrintTimestep(currentTimestep);

		//Check all inServiceMotorcycles of each region, restore all ready ones
		returnMotorcycles(currentTimestep);

		//Execute all events at current timestep
		executeEvents(currentTimestep);

		//Check for auto-promotion of orders
		autoPromoteAll(currentTimestep);

		//Show all active orders in each region
		showActiveOrders();
		pGUI->UpdateInterface();
		pGUI->PrintTimestep(currentTimestep);

		//Display region info (on the status bar)
		displayRegionsData();

		//Send out all orders possible that are in the active Queues/Lists and assign Motorcycles to them
		assignMotorcycles(currentTimestep);

		//Update the interface again, increase the timestep while resetting the list of objects drawn on the screen
		pGUI->UpdateInterface(1);
		pGUI->PrintTimestep(currentTimestep);

		switch (mode)
		{
		case 1:
			pGUI->waitForClick();
			break;
		case 2:
			Sleep(1000);
			break;
		}
		currentTimestep++;
		pGUI->ResetDrawingList();
	}



	pGUI->UpdateInterface();
	pGUI->PrintMessage("Simulation over.");
	
	switch (mode)
	{
	case 1:
		pGUI->waitForClick();
		break;

	case 2:
		Sleep(1000);
		break;
	}

	//Return all motorcycles:
	for (int reg = 0; reg < REGION_COUNT; reg++)
	{
		while (!inServiceMotorcycles[reg].isEmpty())
			returnMotorcycles(++currentTimestep);
	}


	pGUI->PrintMessage("Enter the Output File Name (including .txt):");

	string outputFile = pGUI->GetString();
	writeToFile(outputFile);

	/*
	The function should work as follows:
	Print the current timestep (on the status bar?)
	Check all inServiceMotorcycles of each region, restore all ready ones
	Execute all events at current timestep
	Check for auto-promotion of orders
	Show active orders in each region on the screen (UpdateInterface)
	// Display region info (on the status bar)
	// Display assigned Motorcycles of the last timestep (on the status bar?)
	// Display total amount of orders served of each type (on the status bar?)
	Send out all orders possible that are in the active Queues/Lists and assign Motorcycles to them
	Update the interface again, increase the timestep, reset the list of objects drawn on the screen
	*/

	/*
	Statistics are required at the end of the program (please refer to the project document).
	Ideas for calculating said statistics are yet to be decided on.
	Where do we store the data for served orders?

	---> Universal PriorityQueue for all orders of all regions.
	---> setPriority now takes 0 for VIP priority and 1 for finishTime.
	---> Orders are added after being assigned to motorcycles.
	*/
}

void Restaurant::interactiveMode()
{
	Operate(1);
}

void Restaurant::stepByStepMode()
{
	Operate(2);
}

void Restaurant::silentMode()
{
	pGUI->PrintMessage("Enter the Input File Name (including .txt):");

	string inputFile = pGUI->GetString();
	loadFromFile(inputFile);

	int currentTimestep = 1;
	while (!eventsQueue.isEmpty() || !finished())
	{
		//Print current timestep
		//pGUI->PrintTimestep(currentTimestep);

		//Check all inServiceMotorcycles of each region, restore all ready ones
		returnMotorcycles(currentTimestep);

		//Execute all events at current timestep
		executeEvents(currentTimestep);

		//Check for auto-promotion of orders
		autoPromoteAll(currentTimestep);

		//Show all active orders in each region
		//showActiveOrders();
		//pGUI->UpdateInterface(1);
		//pGUI->PrintTimestep(currentTimestep);

		//Display region info (on the status bar)
		//displayRegionsData();

		//Send out all orders possible that are in the active Queues/Lists and assign Motorcycles to them
		assignMotorcycles(currentTimestep);

		//Update the interface again, increase the timestep while resetting the list of objects drawn on the screen
		//pGUI->UpdateInterface();
		//pGUI->PrintTimestep(currentTimestep);

		//pGUI->waitForClick();
		//pGUI->ResetDrawingList();
		currentTimestep++;
	}



	//pGUI->UpdateInterface();
	pGUI->PrintMessage("Simulation over.");
	//pGUI->waitForClick();

	//Return all motorcycles:
	for (int reg = 0; reg < REGION_COUNT; reg++)
	{
		while (!inServiceMotorcycles[reg].isEmpty())
			returnMotorcycles(++currentTimestep);
	}


	pGUI->PrintMessage("Enter the Output File Name (including .txt):");

	string outputFile = pGUI->GetString();
	writeToFile(outputFile);
}

void Restaurant::loadFromFile(string fileName)
{
	//Reading from the input file:
	ifstream inFile;
	inFile.open(fileName);
	
	// Collecting Motorcycle Data (post-bonus):
	// Suggested method is listing the number of motorcycles first for each type separately.
	// Then listing the ID followed by speed of each individual motorcycle, motorcycles of the same type follow each other.
	// This is repeated four times, one for each region.
	// Example:
	// 1 1 1
	// 1 1 ---> Normal Motorcycle, Region A, ID 1, Speed 1
	// 2 2
	// 3 3
	//
	// 1 1 1
	// 4 2
	// 5 4
	// 6 6
	//
	// 1 0 1
	// 7 3
	// 8 6 ---> Fast Motorcycle, Region C, ID 8, Speed 6
	//
	// 1 0 0
	// 9 4

	int normalMotorCount[REGION_COUNT], frozenMotorCount[REGION_COUNT], vipMotorCount[REGION_COUNT];

	for (int reg = 0; reg < REGION_COUNT; reg++)
	{
		inFile >> normalMotorCount[reg] >> frozenMotorCount[reg] >> vipMotorCount[reg];
				
		int id, speed;

		for (int j = 0; j < normalMotorCount[reg]; j++)
		{
			inFile >> id >> speed;
			Motorcycle *normalMotor = new Motorcycle(id, MOTOR_NORMAL, speed, REGION(reg));
			normalMotorQueue[reg].enqueue(normalMotor);
		}

		for (int j = 0; j < frozenMotorCount[reg]; j++)
		{
			inFile >> id >> speed;
			Motorcycle *frozenMotor = new Motorcycle(id, MOTOR_FROZEN, speed, REGION(reg));
			frozenMotorQueue[reg].enqueue(frozenMotor);
		}

		for (int j = 0; j < vipMotorCount[reg]; j++)
		{
			inFile >> id >> speed;
			Motorcycle *vipMotor = new Motorcycle(id, MOTOR_FAST, speed, REGION(reg));
			vipMotorQueue[reg].enqueue(vipMotor);
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
			toBeAdded = new PromotionEvent;
			break;
		}
		toBeAdded->readData(inFile);
		addEvent(toBeAdded);
	}

	inFile.close();
}

void Restaurant::writeToFile(string filename)
{
	//Opening the file:
	ofstream outFile;
	outFile.open(filename);

	//Storing Data for each region:
	int vipOrderCount[REGION_COUNT] = { 0 };
	int frozenOrderCount[REGION_COUNT] = { 0 };
	int normalOrderCount[REGION_COUNT] = { 0 };
	int totalOrderCount[REGION_COUNT] = { 0 };

	int totalMotorCount[REGION_COUNT] = { 0 };
	
	int waitSum[REGION_COUNT] = { 0 };
	int serviceSum[REGION_COUNT] = { 0 };

	//Writing order info:
	outFile << "FT\tID\tAT\tWT\tST\n";
	while (!totalQueue.isEmpty())
	{
		Order* currentOrder = 0;
		REGION orderRegion = REGION_COUNT;
		ORDER_TYPE orderType = TYPE_COUNT;

		//Write and collect the info to the file and remove the order:
		if (totalQueue.dequeue(currentOrder))
		{
			currentOrder->writeData(outFile);
			orderRegion = currentOrder->GetRegion();
			orderType = currentOrder->GetType();
			
			waitSum[orderRegion] += currentOrder->getWaitTime();
			serviceSum[orderRegion] += currentOrder->getServiceTime();

			switch (orderType)
			{
			case TYPE_VIP:
				vipOrderCount[orderRegion]++;
				break;

			case TYPE_FROZEN:
				frozenOrderCount[orderRegion]++;
				break;

			case TYPE_NORMAL:
				normalOrderCount[orderRegion]++;
				break;
			}

			totalOrderCount[orderRegion]++;			
		}
	}

	//Writing region info:
	for (int reg = 0; reg < REGION_COUNT; reg++)
	{
		totalMotorCount[reg] = vipMotorQueue[reg].getLength() + frozenMotorQueue[reg].getLength() + normalMotorQueue[reg].getLength();

		outFile << "\n\n\nRegion " << char('A' + reg) << ":\n";
		
		outFile << "\tOrders: " << totalOrderCount[reg] 
			<< " [Normal: " << normalOrderCount[reg] 
			<< ", Frozen: " << frozenOrderCount[reg] 
			<< ", VIP: " << vipOrderCount[reg] << "]\n";
		
		outFile << "\tMotorC: " << totalMotorCount[reg]
			<< " [Normal: " << normalMotorQueue[reg].getLength()
			<< ", Frozen: " << frozenMotorQueue[reg].getLength()
			<< ", VIP: " << vipMotorQueue[reg].getLength() << "]\n";

		float averageWait = waitSum[reg] * 1.0 / totalOrderCount[reg];
		float averageService = serviceSum[reg] * 1.0 / totalOrderCount[reg];

		outFile << "\tAverage Wait = " << averageWait << ", " << "Average Service = " << averageService;
	}
	
	//Closing the file:
	outFile.close();
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

		//TODO: Implement the toArray() function to prevent overhead
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

bool Restaurant::promote(int id, int extraMoney)
{
	Order *promotedOrder = orderIdArray[id];
	for (int reg = 0; reg < REGION_COUNT; reg++)
	{
		if (normalQueue[reg].remove(promotedOrder))
		{
			promotedOrder->promote(extraMoney);
			vipQueue[reg].enqueue(promotedOrder);
			return true;
		}
	}
	return false;
}

bool Restaurant::cancel(int id)
{ 
	Order* cancelledOrder = orderIdArray[id];
	
	for (int reg = 0; reg < REGION_COUNT; reg++)
	{
		if (normalQueue[reg].remove(cancelledOrder)) {
			return true;
		}
	}
	return false;
}

void Restaurant::assignOrderToMotorcycle(int currentTime, Order *pOrd, Motorcycle *pMotor)
{
	int orderDistance = -1, motorSpeed = -1;
	if (pOrd && pMotor)
	{
		orderDistance = pOrd->GetDistance();
		motorSpeed = pMotor->getSpeed();
		
		pOrd->setTimes(currentTime, motorSpeed);
		pOrd->setPriority(1);

		pMotor->setFinishTime(currentTime, orderDistance);
		pMotor->setOrderID(pOrd->GetID());
		pMotor->setStatus(SERVICE);
	}
}

void Restaurant::returnMotorcycles(int currentTimestep)
{
	for (int reg = 0; reg < REGION_COUNT; reg++)
	{
		int inServiceLength = inServiceMotorcycles[reg].getLength();

		for (int i = 0; i < inServiceLength; i++)
		{
			Motorcycle *currentMotor = 0;
			inServiceMotorcycles[reg].getEntryAt(i, currentMotor);
			if (!currentMotor)
				continue;

			if (currentMotor->getFinishTime() <= currentTimestep)
			{
				inServiceMotorcycles[reg].remove(currentMotor);
				currentMotor->setStatus(IDLE);

				switch (currentMotor->getType())
				{
				case MOTOR_NORMAL:
					normalMotorQueue[reg].enqueue(currentMotor);
					break;

				case MOTOR_FROZEN:
					frozenMotorQueue[reg].enqueue(currentMotor);
					break;

				case MOTOR_FAST:
					vipMotorQueue[reg].enqueue(currentMotor);
					break;
				}
			}
		}
	}
}

void Restaurant::assignMotorcycles(int currentTimestep)
{
	for (int reg = 0; reg < REGION_COUNT; reg++)
	{
		//First the VIP orders:
		while (!vipQueue[reg].isEmpty() && !vipMotorQueue[reg].isEmpty())
		{
			Order *pOrd;
			Motorcycle *pMotor;

			vipQueue[reg].dequeue(pOrd);
			vipMotorQueue[reg].dequeue(pMotor);
			
			assignOrderToMotorcycle(currentTimestep, pOrd, pMotor);

			totalQueue.enqueue(pOrd);
			inServiceMotorcycles[reg].append(pMotor);
		}

		while (!vipQueue[reg].isEmpty() && !normalMotorQueue[reg].isEmpty())
		{
			Order *pOrd;
			Motorcycle *pMotor;

			vipQueue[reg].dequeue(pOrd);
			normalMotorQueue[reg].dequeue(pMotor);

			assignOrderToMotorcycle(currentTimestep, pOrd, pMotor);

			totalQueue.enqueue(pOrd);
			inServiceMotorcycles[reg].append(pMotor);
		}

		while (!vipQueue[reg].isEmpty() && !frozenMotorQueue[reg].isEmpty())
		{
			Order *pOrd;
			Motorcycle *pMotor;

			vipQueue[reg].dequeue(pOrd);
			frozenMotorQueue[reg].dequeue(pMotor);

			assignOrderToMotorcycle(currentTimestep, pOrd, pMotor);

			totalQueue.enqueue(pOrd);
			inServiceMotorcycles[reg].append(pMotor);
		}

		/////////////////////////////////////////////////////////////////////////////

		//Then the Frozen orders:
		while (!frozenQueue[reg].isEmpty() && !frozenMotorQueue[reg].isEmpty())
		{
			Order *pOrd;
			Motorcycle *pMotor;

			frozenQueue[reg].dequeue(pOrd);
			frozenMotorQueue[reg].dequeue(pMotor);

			assignOrderToMotorcycle(currentTimestep, pOrd, pMotor);

			totalQueue.enqueue(pOrd);
			inServiceMotorcycles[reg].append(pMotor);
		}

		////////////////////////////////////////////////////////////////////////////

		//And finally the Normal orders:
		while (!normalQueue[reg].isEmpty() && !normalMotorQueue[reg].isEmpty())
		{
			Order *pOrd;
			Motorcycle *pMotor;

			normalQueue[reg].pop(pOrd);
			normalMotorQueue[reg].dequeue(pMotor);

			assignOrderToMotorcycle(currentTimestep, pOrd, pMotor);

			totalQueue.enqueue(pOrd);
			inServiceMotorcycles[reg].append(pMotor);
		}

		while (!normalQueue[reg].isEmpty() && !vipMotorQueue[reg].isEmpty())
		{
			Order *pOrd;
			Motorcycle *pMotor;

			normalQueue[reg].pop(pOrd);
			vipMotorQueue[reg].dequeue(pMotor);

			assignOrderToMotorcycle(currentTimestep, pOrd, pMotor);

			totalQueue.enqueue(pOrd);
			inServiceMotorcycles[reg].append(pMotor);
		}
	}
}