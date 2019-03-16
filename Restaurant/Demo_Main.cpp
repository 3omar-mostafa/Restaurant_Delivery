//#include "Drawing.h"
#include "Restaurant\Restaurant.h"
#include "GUI\GUI.h"

int main()
{
	
	Restaurant* pRest = new Restaurant;
	pRest->runSimulation();
	
	delete pRest;
	
	return 0;
}
