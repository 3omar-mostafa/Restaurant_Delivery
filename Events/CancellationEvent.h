#pragma once
#include "Event.h"
class CancellationEvent :
	public Event
{
public:
	CancellationEvent(int eTime = -1, int ordID = -1);
	~CancellationEvent();

	void readData(ifstream& inFile) override;
	void execute(Restaurant* pRest) override;
};

