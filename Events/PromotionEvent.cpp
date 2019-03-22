#include "PromotionEvent.h"
#include "..\Restaurant\Restaurant.h"



PromotionEvent::PromotionEvent(int eTime, int ordID, double eMoney) : Event (eTime,ordID)
{
	extraMoney = eMoney;
}

void PromotionEvent::execute(Restaurant * pRest)
{
	//pRest->(pOrd);
}


PromotionEvent::~PromotionEvent()
{
}
