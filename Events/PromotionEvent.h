#pragma once
#include "Event.h"
#include "../Restaurant/Order.h"
class PromotionEvent :
	public Event
{
	double extraMoney;
public:
	PromotionEvent(int eTime, int ordID, double extraMoney);
	void execute(Restaurant *pRest) override;
	~PromotionEvent();
};

