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
bool isGreaterThan(T *left, T *right)
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

Restaurant::~Restaurant()
{
	delete pGUI;
}

int Restaurant::getTimeStep()
{
	return timeStep;
}

////////////////////////////////  Main Program Functions   /////////////////////////////

void Restaurant::runSimulation()
{
	pGUI = new GUI;
	PROGRAM_MODE mode = pGUI->getGUIMode();
	Operate(mode);
}

void Restaurant::Operate(PROGRAM_MODE mode)
{
	pGUI->PrintMessage("Enter the Input File Name:");

	string inputFile = "Test_Cases\\" + pGUI->GetString() + ".txt";
	loadFromFile(inputFile);

	if (mode == MODE_RAMADAN)
	{
		pGUI->PrintMessage("No Orders will be served between Fajr and Maghreb - Ramadan Kareem");
		PlaySound(TEXT("Restaurant\\Ramadan\\Wahawy_ya_wahawy.wav"), nullptr, SND_FILENAME);
	}

	int currentTimestep = 1;
	while (!eventsQueue.isEmpty() || !finished())
	{
		timeStep = currentTimestep;
		//Check all inServiceMotorcycles of each region, restore all ready ones
		returnMotorcycles(currentTimestep);

		//Execute all events at current timestep
		executeEvents(currentTimestep);

		//Check for auto-promotion of orders
		autoPromoteAll(currentTimestep);

		if (mode != MODE_SILENT)
		{
			if (mode == MODE_RAMADAN)
				Ramadan(currentTimestep);

			//Display region info (on the status bar)
			displayRegionsData();

			//Show all active orders in each region
			showActiveOrders();

			int totalOrders = 0;
			for (int reg = 0; reg < REGION_COUNT; reg++)
				totalOrders += normalQueue[reg].getLength() + vipQueue[reg].getLength() + frozenQueue[reg].getLength();

			pGUI->UpdateInterface(totalOrders < 20, currentTimestep);
			pGUI->PrintTimestep(currentTimestep);
		}

		//Send out all orders possible that are in the active Queues/Lists and assign Motorcycles to them
		if (mode != MODE_RAMADAN || (mode == MODE_RAMADAN && currentTimestep % 24 >= 19 || currentTimestep % 24 < 3))
		{
			assignMotorcycles(currentTimestep);
			if (mode != MODE_SILENT)
			{
				Sleep(100);
				pGUI->OrderOut(currentTimestep);
			}
		}

		//Increment the timestep
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

			// Delay Time between Asr and Maghreb Athan as we feel in real life
			if (currentTimestep % 24 >= 15 && currentTimestep % 24 < 19)
				Sleep(2000);

			pGUI->ResetDrawingList();
			break;
		}

		currentTimestep++;
	}

	if (mode != MODE_SILENT)
	{
		pGUI->UpdateInterface(false, currentTimestep);
		displayRegionsData();
	}

	switch (mode)
	{
	case MODE_INTERACTIVE:
		pGUI->waitForClick();
		break;

	case MODE_STEP:
	case MODE_RAMADAN:
		Sleep(1000);
		break;
	}

	//Return all motorcycles:
	for (int reg = 0; reg < REGION_COUNT; reg++)
	{
		while (!inServiceMotorcycles[reg].isEmpty())
			returnMotorcycles(++currentTimestep);
	}

	if (mode != MODE_SILENT)
	{
		pGUI->PrintMessage("Simulation over. Click to continue.");
		pGUI->waitForClick();
	}

	pGUI->PrintMessage("Enter the Output File Name:");

	string outputFile = pGUI->GetString() + ".txt";
	writeToFile(outputFile);

	outputFile = "start " + outputFile;
	system(outputFile.c_str());

	/*
	
	The function should work as follows:
	// Check all inServiceMotorcycles of each region, restore all ready ones
	// Execute all events at current timestep
	// Check for auto-promotion of orders
	// Display region info (on the status bar)
	// Display assigned Motorcycles of the last timestep (on the status bar)
	// Display total amount of orders served of each type (on the status bar)
	// Show active orders in each region on the screen (UpdateInterface)
	// Send out all orders possible that are in the active Queues/Lists and assign Motorcycles to them
	// Update the interface
	// Increment the Timestep
	// Repeat until there are no more events or orders
	
	Statistics are required at the end of the program.
	Where do we store the data for served orders?
	---> Universal PriorityQueue for all orders of all regions.
	---> setPriority now takes 0 for VIP priority and 1 for -finishTime.
	---> Orders are added after being assigned to motorcycles.

	*/
}

// Plays Festive Music and Athans
void Restaurant::Ramadan(int currentTimestep)
{
	int t = currentTimestep % 24;
	image img;

	switch (t)
	{
	case 3:
		img.Open("Restaurant\\Ramadan\\fajr.jpg");
		pGUI->drawImage(img, 0, 0);
		PlaySound(TEXT("Restaurant\\Ramadan\\Athan_Fajr.wav"), nullptr, SND_SYNC);
		break;

	case 4:
		for (int reg = 0; reg < REGION_COUNT; reg++)
			assignedMotorcyclesLastTimestep[reg].clear();

		PlaySound(TEXT("Restaurant\\Ramadan\\Ramadan_gana.wav"), nullptr, SND_ASYNC);
		break;

	case 12:
		PlaySound(TEXT("Restaurant\\Ramadan\\Mar7ab_shahr_elsom.wav"), nullptr, SND_ASYNC);
		break;

	case 19:
		Sleep(5000);
		img.Open("Restaurant\\Ramadan\\maghreb.jpg");
		pGUI->drawImage(img, 0, 0);
		PlaySound(TEXT("Restaurant\\Ramadan\\Athan_Maghreb.wav"), nullptr, SND_SYNC);
		break;

	case 20:
		PlaySound(TEXT("Restaurant\\Ramadan\\Aho_geh_ya_wlad.wav"), nullptr, SND_ASYNC);
		break;
	}
}

////////////////////////////////  Event Handling Functions   /////////////////////////////

//Adds a new event to the queue of events
void Restaurant::addEvent(Event *pE)
{
	eventsQueue.enqueue(pE);
}

//Executes ALL events that should take place at current timestep
void Restaurant::executeEvents(int CurrentTimeStep)
{
	Event *pE;
	while (eventsQueue.peekFront(pE)) //as long as there are more events
	{
		if (pE->getEventTime() > CurrentTimeStep) //no more events at current time
			return;

		pE->execute(this);
		eventsQueue.dequeue(pE); //remove event from the queue
		delete pE;				 //deallocate event object from memory
	}
}

//////////////////////////////////  File Handling Functions   /////////////////////////////

void Restaurant::loadFromFile(string fileName)
{
	//Reading from the input file:
	ifstream inFile;
	inFile.open(fileName);

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
		Event *toBeAdded;
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
	int vipOrderCount[REGION_COUNT] = {0};
	int frozenOrderCount[REGION_COUNT] = {0};
	int normalOrderCount[REGION_COUNT] = {0};
	int totalOrderCount[REGION_COUNT] = {0};

	int totalMotorCount[REGION_COUNT] = {0};

	int waitSum[REGION_COUNT] = {0};
	int serviceSum[REGION_COUNT] = {0};

	//Writing order info:
	outFile << "FT   ID   AT   WT   ST \n";
	while (!totalQueue.isEmpty())
	{
		Order *currentOrder = 0;
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

		delete currentOrder;
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

		float averageWait = totalOrderCount[reg] ? waitSum[reg] * 1.0 / totalOrderCount[reg] : 0;
		float averageService = totalOrderCount[reg] ? serviceSum[reg] * 1.0 / totalOrderCount[reg] : 0;

		outFile << "\tAverage Wait = " << averageWait << ", "
				<< "Average Service = " << averageService;
	}

	//Closing the file:
	outFile.close();
}

//////////////////////////////////  Queue Handling functions   /////////////////////////////

//Adds the order to its proper Queue
void Restaurant::addToActiveQueue(Order *pOrd)
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
		Order *pOrd = nullptr;

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

		Order **arr = vipQueue[reg].toArray();
		int length = vipQueue[reg].getLength();
		for (int i = 0; i < length; ++i)
			pGUI->AddOrderForDrawing(arr[i]);

		delete[] arr;
	}
}

// Checks if there are no more orders in all queues.
bool Restaurant::finished() const
{
	for (int reg = 0; reg < REGION_COUNT; reg++)
		if (!(normalQueue[reg].isEmpty() && frozenQueue[reg].isEmpty() && vipQueue[reg].isEmpty()))
			return false;
	return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Displays Regions Info on the status bar
void Restaurant::displayRegionsData()
{
	string regionsOrderData[REGION_COUNT] = {""};
	string regionsMotorCyclesData[REGION_COUNT] = {""};
	string ordersServed[REGION_COUNT] = {""};

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

// Returns a reference to the order of the passed ID
Order *&Restaurant::orderOfID(int i)
{
	return orderIdArray[i];
}

//////////////////////////////////  Order Handling functions   /////////////////////////////

// Returns true if an order was promoted in a region
bool Restaurant::autoPromoteRegion(int currentTimeStep, REGION reg)
{
	Order *toBePromoted;
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

// Promotes all orders in all regions
void Restaurant::autoPromoteAll(int currentTimeStep)
{
	for (int i = 0; i < REGION_COUNT; i++)
		while (autoPromoteRegion(currentTimeStep, REGION(i)))
			;
}

// Promotes an individual order, returns true if successful
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

// Cancels an individual order, returns true if successful
bool Restaurant::cancel(int id,int time)
{
	Order *cancelledOrder = orderIdArray[id];
	for (int reg = 0; reg < REGION_COUNT; reg++)
	{
		if (normalQueue[reg].remove(cancelledOrder))
		{
			pGUI->DestroyOrder(cancelledOrder,time);
			return true;
		}
	}
	return false;
}

//////////////////////////////////  Motorcycle Handling functions   /////////////////////////////

// Links the Order to the Motorcycle, sets times for both
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

// Assigns all available Orders to all available Motorcycles
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
			if (pMotor->getHP() == 0)
			{
				damagedMotorQueue[reg].enqueue(pMotor);
				pMotor->setHP(5);
				continue;
			}

			vipQueue[reg].dequeue(pOrd);

			assignOrderToMotorcycle(currentTimestep, pOrd, pMotor);
			assignedMotorcyclesLastTimestep[reg] += assignedMotorcyclesData(pMotor, pOrd);

			totalQueue.enqueue(pOrd);
			totalOrdersServed[reg][TYPE_VIP]++;

			pMotor->setHP(pMotor->getHP() - 1);
			inServiceMotorcycles[reg].append(pMotor);
		}

		while (!vipQueue[reg].isEmpty() && !normalMotorQueue[reg].isEmpty())
		{
			Order *pOrd;
			Motorcycle *pMotor;

			normalMotorQueue[reg].dequeue(pMotor);
			if (pMotor->getHP() == 0)
			{
				damagedMotorQueue[reg].enqueue(pMotor);
				pMotor->setHP(5);
				continue;
			}

			vipQueue[reg].dequeue(pOrd);

			assignOrderToMotorcycle(currentTimestep, pOrd, pMotor);
			assignedMotorcyclesLastTimestep[reg] += assignedMotorcyclesData(pMotor, pOrd);

			totalQueue.enqueue(pOrd);
			totalOrdersServed[reg][TYPE_VIP]++;

			pMotor->setHP(pMotor->getHP() - 1);
			inServiceMotorcycles[reg].append(pMotor);
		}

		while (!vipQueue[reg].isEmpty() && !frozenMotorQueue[reg].isEmpty())
		{
			Order *pOrd;
			Motorcycle *pMotor;

			frozenMotorQueue[reg].dequeue(pMotor);
			if (pMotor->getHP() == 0)
			{
				damagedMotorQueue[reg].enqueue(pMotor);
				pMotor->setHP(5);
				continue;
			}

			vipQueue[reg].dequeue(pOrd);

			assignOrderToMotorcycle(currentTimestep, pOrd, pMotor);
			assignedMotorcyclesLastTimestep[reg] += assignedMotorcyclesData(pMotor, pOrd);

			totalQueue.enqueue(pOrd);
			totalOrdersServed[reg][TYPE_VIP]++;

			pMotor->setHP(pMotor->getHP() - 1);
			inServiceMotorcycles[reg].append(pMotor);
		}

		/////////////////////////////////////////////////////////////////////////////

		//Then the Frozen orders:
		while (!frozenQueue[reg].isEmpty() && !frozenMotorQueue[reg].isEmpty())
		{
			Order *pOrd;
			Motorcycle *pMotor;

			frozenMotorQueue[reg].dequeue(pMotor);
			if (pMotor->getHP() == 0)
			{
				damagedMotorQueue[reg].enqueue(pMotor);
				pMotor->setHP(5);
				continue;
			}

			frozenQueue[reg].dequeue(pOrd);

			assignOrderToMotorcycle(currentTimestep, pOrd, pMotor);
			assignedMotorcyclesLastTimestep[reg] += assignedMotorcyclesData(pMotor, pOrd);

			totalQueue.enqueue(pOrd);
			totalOrdersServed[reg][TYPE_FROZEN]++;

			pMotor->setHP(pMotor->getHP() - 1);
			inServiceMotorcycles[reg].append(pMotor);
		}

		////////////////////////////////////////////////////////////////////////////

		//And finally the Normal orders:
		while (!normalQueue[reg].isEmpty() && !normalMotorQueue[reg].isEmpty())
		{
			Order *pOrd;
			Motorcycle *pMotor;

			normalMotorQueue[reg].dequeue(pMotor);
			if (pMotor->getHP() == 0)
			{
				damagedMotorQueue[reg].enqueue(pMotor);
				pMotor->setHP(5);
				continue;
			}

			normalQueue[reg].pop(pOrd);

			assignOrderToMotorcycle(currentTimestep, pOrd, pMotor);
			assignedMotorcyclesLastTimestep[reg] += assignedMotorcyclesData(pMotor, pOrd);

			totalQueue.enqueue(pOrd);
			totalOrdersServed[reg][TYPE_NORMAL]++;

			pMotor->setHP(pMotor->getHP() - 1);
			inServiceMotorcycles[reg].append(pMotor);
		}

		while (!normalQueue[reg].isEmpty() && !vipMotorQueue[reg].isEmpty())
		{
			Order *pOrd;
			Motorcycle *pMotor;

			vipMotorQueue[reg].dequeue(pMotor);
			if (pMotor->getHP() == 0)
			{
				damagedMotorQueue[reg].enqueue(pMotor);
				pMotor->setHP(5);
				continue;
			}

			normalQueue[reg].pop(pOrd);

			assignOrderToMotorcycle(currentTimestep, pOrd, pMotor);
			assignedMotorcyclesLastTimestep[reg] += assignedMotorcyclesData(pMotor, pOrd);

			totalQueue.enqueue(pOrd);
			totalOrdersServed[reg][TYPE_NORMAL]++;

			pMotor->setHP(pMotor->getHP() - 1);
			inServiceMotorcycles[reg].append(pMotor);
		}
	}
}

// Moves Motorcycles from damagedMotorQueue/inServiceMotorcycles to MotorQueues
void Restaurant::returnMotorcycles(int currentTimestep)
{
	for (int reg = 0; reg < REGION_COUNT; reg++)
	{
		for (int i = 0; i < 3; i++)
		{
			Motorcycle *currMotor = nullptr;
			if (damagedMotorQueue[reg].peekFront(currMotor))
			{
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

// Get data of Order and its Motorcycle to display it later
string Restaurant::assignedMotorcyclesData(Motorcycle *pMotor, Order *pOrd) const
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
