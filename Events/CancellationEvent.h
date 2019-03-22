#pragma once
#include "Event.h"
class CancellationEvent :
	public Event
{
public:
	CancellationEvent(int eTime = -1, int ordID = -1);
	//CancellationEvent();
	~CancellationEvent();

	void readData(ifstream& inFile);
	void execute(Restaurant* pRest);
};

