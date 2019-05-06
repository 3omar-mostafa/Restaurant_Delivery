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
	pGUI = nullptr;
	for (int reg = 0; reg < REGION_COUNT; ++reg)
		for (int type = 0; type < TYPE_COUNT; ++type)
			totalOrdersServed[reg][type] = 0;
}

void Restaurant::runSimulation()
{
	pGUI = new GUI;
	PROGRAM_MODE mode = pGUI->getGUIMode();
	Operate(mode);

	//while (1)	readColor();

}

void Restaurant::readColor()
{
	pGUI->PrintMessage("Enter Hex Code:");
	string hexCode = pGUI->GetString();
	color newColor(hexCode);
	pGUI->UpdateInterface(newColor);
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
	string regionsOrderData[REGION_COUNT] = { "" };
	string regionsMotorCyclesData[REGION_COUNT] = { "" };
	string ordersServed[REGION_COUNT] = { "" };

	for (int reg = 0; reg < REGION_COUNT; reg++)
	{
		int noActiveOrdersOf[TYPE_COUNT] = { 0 };
		int noAvailableMotor[TYPE_COUNT] = { 0 };

		noActiveOrdersOf[TYPE_NORMAL] += normalQueue[reg].getLength();
		noActiveOrdersOf[TYPE_FROZEN] += frozenQueue[reg].getLength();
		noActiveOrdersOf[TYPE_VIP] += vipQueue[reg].getLength();

		noAvailableMotor[TYPE_NORMAL] += normalMotorQueue[reg].getLength();
		noAvailableMotor[TYPE_FROZEN] += frozenMotorQueue[reg].getLength();
		noAvailableMotor[TYPE_VIP] += vipMotorQueue[reg].getLength();

		regionsOrderData[reg] += to_string(noActiveOrdersOf[TYPE_NORMAL]) + " Normal Orders             ";
		regionsOrderData[reg] += to_string(noActiveOrdersOf[TYPE_FROZEN]) + " Frozen Orders             ";
		regionsOrderData[reg] += to_string(noActiveOrdersOf[TYPE_VIP]) + " VIP Orders ";

		regionsMotorCyclesData[reg] += to_string(noAvailableMotor[TYPE_NORMAL]) + " Normal Motorcycles     ";
		regionsMotorCyclesData[reg] += to_string(noAvailableMotor[TYPE_FROZEN]) + " Frozen Motorcycles     ";
		regionsMotorCyclesData[reg] += to_string(noAvailableMotor[TYPE_VIP]) + " VIP Motorcycles";

		ordersServed[reg] += "Total Served Orders : ";
		ordersServed[reg] += to_string(totalOrdersServed[reg][TYPE_NORMAL]) + " Normal    ";
		ordersServed[reg] += to_string(totalOrdersServed[reg][TYPE_FROZEN]) + " Frozen    ";
		ordersServed[reg] += to_string(totalOrdersServed[reg][TYPE_VIP]) + " VIP    ";
	}
	pGUI->PrintRegions(regionsOrderData, regionsMotorCyclesData, assignedMotorcyclesLastTimestep, ordersServed);
}

string Restaurant::assignedMotorcyclesData(Motorcycle* pMotor, Order* pOrd) const
{
	string s;

	switch (pMotor->getType())
	{
	case MOTOR_FAST:
		s += "V";
		break;
	case MOTOR_NORMAL:
		s += "N";
		break;
	case MOTOR_FROZEN:
		s += "F";
		break;
	}

	s += to_string(pMotor->getID()) + "(";

	switch (pOrd->getType())
	{
	case TYPE_VIP:
		s += "V";
		break;
	case TYPE_NORMAL:
		s += "N";
		break;
	case TYPE_FROZEN:
		s += "F";
		break;
	}

	s += to_string(pOrd->getID()) + ")    ";

	return s;
}

void Restaurant::Operate(PROGRAM_MODE mode)
{
	pGUI->PrintMessage("Enter the Input File Name:");

	string inputFile = pGUI->GetString() + ".txt";
	loadFromFile(inputFile);

	if (mode == MODE_RAMADAN) {
		pGUI->PrintMessage("No Orders will be served between Fajr and Maghreb - Ramadan Kareem");
		Sleep(5000);
	}

	int currentTimestep = 1;
	while (!eventsQueue.isEmpty() || !finished())
	{

		//Check all inServiceMotorcycles of each region, restore all ready ones
		returnMotorcycles(currentTimestep);

		//Execute all events at current timestep
		executeEvents(currentTimestep);

		//Check for auto-promotion of orders
		autoPromoteAll(currentTimestep);

		if (mode != MODE_SILENT)
		{
			//Print current timestep
			pGUI->PrintTimestep(currentTimestep);

			//Display region info (on the status bar)
			displayRegionsData();

			//Show all active orders in each region
			showActiveOrders();
			//TODO: mistake in this condition: it will check only queues of Region A
			if (normalQueue->getLength() + vipQueue->getLength() + frozenQueue->getLength() > 20) {
				pGUI->UpdateInterface(true, currentTimestep);
			}
			else
				pGUI->UpdateInterface(false, currentTimestep);
			pGUI->PrintTimestep(currentTimestep);
		}

		if (mode == MODE_RAMADAN && currentTimestep % 24 == 19) {
			image img("Restaurant\\Ramadan\\maghreb.jpg");
			pGUI->drawImage(img, 0, 0);
			PlaySound(TEXT("Restaurant\\Ramadan\\Athan_Maghreb.wav"), nullptr, SND_ASYNC);
			Sleep(7000);
		}

		if (mode == MODE_RAMADAN && currentTimestep % 24 == 3) {
			image img("Restaurant\\Ramadan\\fajr.jpg");
			pGUI->drawImage(img, 0, 0);
			PlaySound(TEXT("Restaurant\\Ramadan\\Athan_Fajr.wav"), nullptr, SND_ASYNC);
			Sleep(2000);
		}

		if (mode != MODE_RAMADAN || (mode == MODE_RAMADAN && currentTimestep % 24 >= 19 || currentTimestep % 24 <= 3)) {
			//Send out all orders possible that are in the active Queues/Lists and assign Motorcycles to them
			assignMotorcycles(currentTimestep);
		}

		switch (mode)
		{
		case MODE_INTERACTIVE:
			pGUI->waitForClick();
			pGUI->ResetDrawingList();
			break;

		case MODE_STEP:
			Sleep(1000);
			pGUI->ResetDrawingList();
			break;

		case MODE_RAMADAN:
			Sleep(2000);
			pGUI->ResetDrawingList();
			break;
		}
		currentTimestep++;
	}

	if (mode != MODE_SILENT)
		pGUI->UpdateInterface();

	pGUI->PrintMessage("Simulation over.");

	switch (mode)
	{
	case MODE_INTERACTIVE:
		pGUI->waitForClick();
		break;

	case MODE_STEP:
		Sleep(1000);
		break;
	}

	//Return all motorcycles:
	for (int reg = 0; reg < REGION_COUNT; reg++)
	{
		while (!inServiceMotorcycles[reg].isEmpty())
			returnMotorcycles(++currentTimestep);
	}


	pGUI->PrintMessage("Enter the Output File Name:");

	string outputFile = pGUI->GetString() + ".txt";
	writeToFile(outputFile);

	outputFile = "start " + outputFile;
	system(outputFile.c_str());

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
			orderRegion = currentOrder->getRegion();
			orderType = currentOrder->getType();

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

		outFile << "\tMotorcycles: " << totalMotorCount[reg]
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
	REGION reg = pOrd->getRegion();
	switch (pOrd->getType())
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
		Order* pOrd = nullptr;

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

		Order** arr = vipQueue[reg].toArray();
		int length = vipQueue[reg].getLength();
		for (int i = 0; i < length; ++i)
			pGUI->AddOrderForDrawing(arr[i]);

		delete[] arr;
	}
}

bool Restaurant::finished() const
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
	//cancelledOrder->deleteIt(1);
	for (int reg = 0; reg < REGION_COUNT; reg++)
	{
		if (normalQueue[reg].remove(cancelledOrder)) {
			return true;
		}
	}
	return false;
}

void Restaurant::assignOrderToMotorcycle(int currentTime, Order *pOrd, Motorcycle *pMotor) const
{
	int orderDistance = -1, motorSpeed = -1;
	if (pOrd && pMotor)
	{
		orderDistance = pOrd->getDistance();
		motorSpeed = pMotor->getSpeed();

		pOrd->setTimes(currentTime, motorSpeed);
		pOrd->setPriority(1);

		pMotor->setFinishTime(currentTime, orderDistance);
		pMotor->setOrderID(pOrd->getID());
		pMotor->setStatus(SERVICE);
	}
}

void Restaurant::returnMotorcycles(int currentTimestep)
{
	for (int reg = 0; reg < REGION_COUNT; reg++)
	{
		for (int i = 0; i < 3; i++) {
			Motorcycle *currMotor = nullptr;
			if (damagedMotorQueue[reg].peekFront(currMotor)) {
				switch (currMotor->getType())
				{
				case MOTOR_NORMAL:
					normalMotorQueue[reg].enqueue(currMotor);
					break;
				case MOTOR_FROZEN:
					frozenMotorQueue[reg].enqueue(currMotor);
					break;
				case MOTOR_FAST:
					vipMotorQueue[reg].enqueue(currMotor);
					break;
				}
				damagedMotorQueue[reg].dequeue(currMotor);
			}
		}

		int inServiceLength = inServiceMotorcycles[reg].getLength();

		for (int i = 0; i < inServiceLength; i++)
		{
			Motorcycle *currentMotor = nullptr;
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
				i--, inServiceLength--;
			}
		}
	}
}

void Restaurant::assignMotorcycles(int currentTimestep)
{

	for (int reg = 0; reg < REGION_COUNT; reg++)
	{
		assignedMotorcyclesLastTimestep[reg].clear();
	}

	for (int reg = 0; reg < REGION_COUNT; reg++)
	{
		//First the VIP orders:
		while (!vipQueue[reg].isEmpty() && !vipMotorQueue[reg].isEmpty())
		{
			Order *pOrd;
			Motorcycle *pMotor;

			vipMotorQueue[reg].dequeue(pMotor);
			if (pMotor->getHP() == 0) {
				damagedMotorQueue[reg].enqueue(pMotor);
				pMotor->setHP(5);
				continue;
			}

			vipQueue[reg].dequeue(pOrd);
			assignOrderToMotorcycle(currentTimestep, pOrd, pMotor);
			pMotor->setHP(pMotor->getHP() - 1);
			totalQueue.enqueue(pOrd);
			inServiceMotorcycles[reg].append(pMotor);
			assignedMotorcyclesLastTimestep[reg] += assignedMotorcyclesData(pMotor, pOrd);
			totalOrdersServed[reg][TYPE_VIP]++;
		}

		while (!vipQueue[reg].isEmpty() && !normalMotorQueue[reg].isEmpty())
		{
			Order *pOrd;
			Motorcycle *pMotor;

			normalMotorQueue[reg].dequeue(pMotor);
			if (pMotor->getHP() == 0) {
				damagedMotorQueue[reg].enqueue(pMotor);
				pMotor->setHP(5);
				continue;
			}

			vipQueue[reg].dequeue(pOrd);
			assignOrderToMotorcycle(currentTimestep, pOrd, pMotor);
			pMotor->setHP(pMotor->getHP() - 1);
			totalQueue.enqueue(pOrd);
			inServiceMotorcycles[reg].append(pMotor);
			assignedMotorcyclesLastTimestep[reg] += assignedMotorcyclesData(pMotor, pOrd);
			totalOrdersServed[reg][TYPE_VIP]++;
		}

		while (!vipQueue[reg].isEmpty() && !frozenMotorQueue[reg].isEmpty())
		{
			Order *pOrd;
			Motorcycle *pMotor;

			frozenMotorQueue[reg].dequeue(pMotor);
			if (pMotor->getHP() == 0) {
				damagedMotorQueue[reg].enqueue(pMotor);
				pMotor->setHP(5);
				continue;
			}

			vipQueue[reg].dequeue(pOrd);
			assignOrderToMotorcycle(currentTimestep, pOrd, pMotor);
			pMotor->setHP(pMotor->getHP() - 1);
			totalQueue.enqueue(pOrd);
			inServiceMotorcycles[reg].append(pMotor);
			assignedMotorcyclesLastTimestep[reg] += assignedMotorcyclesData(pMotor, pOrd);
			totalOrdersServed[reg][TYPE_VIP]++;
		}

		/////////////////////////////////////////////////////////////////////////////

		//Then the Frozen orders:
		while (!frozenQueue[reg].isEmpty() && !frozenMotorQueue[reg].isEmpty())
		{
			Order *pOrd;
			Motorcycle *pMotor;


			frozenMotorQueue[reg].dequeue(pMotor);
			if (pMotor->getHP() == 0) {
				damagedMotorQueue[reg].enqueue(pMotor);
				pMotor->setHP(5);
				continue;
			}

			frozenQueue[reg].dequeue(pOrd);
			assignOrderToMotorcycle(currentTimestep, pOrd, pMotor);
			pMotor->setHP(pMotor->getHP() - 1);
			totalQueue.enqueue(pOrd);
			inServiceMotorcycles[reg].append(pMotor);
			assignedMotorcyclesLastTimestep[reg] += assignedMotorcyclesData(pMotor, pOrd);
			totalOrdersServed[reg][TYPE_FROZEN]++;
		}

		////////////////////////////////////////////////////////////////////////////

		//And finally the Normal orders:
		while (!normalQueue[reg].isEmpty() && !normalMotorQueue[reg].isEmpty())
		{
			Order *pOrd;
			Motorcycle *pMotor;


			normalMotorQueue[reg].dequeue(pMotor);
			if (pMotor->getHP() == 0) {
				damagedMotorQueue[reg].enqueue(pMotor);
				pMotor->setHP(5);
				continue;
			}

			normalQueue[reg].pop(pOrd);
			assignOrderToMotorcycle(currentTimestep, pOrd, pMotor);
			pMotor->setHP(pMotor->getHP() - 1);
			totalQueue.enqueue(pOrd);
			inServiceMotorcycles[reg].append(pMotor);
			assignedMotorcyclesLastTimestep[reg] += assignedMotorcyclesData(pMotor, pOrd);
			totalOrdersServed[reg][TYPE_NORMAL]++;
		}

		while (!normalQueue[reg].isEmpty() && !vipMotorQueue[reg].isEmpty())
		{
			Order *pOrd;
			Motorcycle *pMotor;


			vipMotorQueue[reg].dequeue(pMotor);
			if (pMotor->getHP() == 0) {
				damagedMotorQueue[reg].enqueue(pMotor);
				pMotor->setHP(5);
				continue;
			}

			normalQueue[reg].pop(pOrd);
			assignOrderToMotorcycle(currentTimestep, pOrd, pMotor);
			pMotor->setHP(pMotor->getHP() - 1);
			totalQueue.enqueue(pOrd);
			inServiceMotorcycles[reg].append(pMotor);
			assignedMotorcyclesLastTimestep[reg] += assignedMotorcyclesData(pMotor, pOrd);
			totalOrdersServed[reg][TYPE_NORMAL]++;
		}
	}
}