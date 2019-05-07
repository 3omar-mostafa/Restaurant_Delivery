#include "PromotionEvent.h"
#include "..\Restaurant\Restaurant.h"

PromotionEvent::PromotionEvent(int eTime, int ordID, double eMoney) : Event(eTime, ordID)
{
	extraMoney = eMoney;
}

PromotionEvent::PromotionEvent() : Event(-1, -1)
{
}

void PromotionEvent::readData(ifstream &inFile)
{
	inFile >> eventTime >> orderID >> extraMoney;
}

void PromotionEvent::execute(Restaurant *pRest)
{
	pRest->promote(orderID, extraMoney);
}
