#include "GUI.h"

//////////////////////////////////////////////////////////////////////////////////////////
GUI::GUI()
{
	pWind = new window(WindWidth + 15, WindHeight, 0, 0);
	pWind->ChangeTitle("The Restaurant");

	OrderCount = 0;

	//Set color for each order type
	OrdersClrs[TYPE_NORMAL] = DARKBLUE; //normal-order color
	OrdersClrs[TYPE_FROZEN] = VIOLET;   //Frozen-order color
	OrdersClrs[TYPE_VIP] = RED;			//VIP-order color

	ClearStatusBar();
	ClearDrawingArea();
	DrawRestArea();
}
//////////////////////////////////////////////////////////////////////////////////////////
GUI::~GUI()
{
	delete pWind;
}

//////////////////////////////////////////////////////////////////////////////////////////
// ================================== INPUT FUNCTIONS ====================================
//////////////////////////////////////////////////////////////////////////////////////////

void GUI::waitForClick() const
{
	int x, y;
	pWind->WaitMouseClick(x, y); //Wait for mouse click
}
//////////////////////////////////////////////////////////////////////////////////////////
string GUI::GetString() const
{
	string Label;
	char Key;
	while (true)
	{
		pWind->WaitKeyPress(Key);
		if (Key == 27) //ESCAPE key is pressed
			return ""; //returns nothing as user has cancelled label
		if (Key == 13) //ENTER key is pressed
			return Label;
		if ((Key == 8) && (!Label.empty())) //BackSpace is pressed
			Label.resize(Label.size() - 1);
		else
			Label += Key;

		PrintMessage(Label);
	}
}

//////////////////////////////////////////////////////////////////////////////////////////
// ================================== OUTPUT FUNCTIONS ===================================
//////////////////////////////////////////////////////////////////////////////////////////

void GUI::PrintMessage(string msg) const //Prints a message on status bar
{
	ClearStatusBar(); //First clear the status bar

	pWind->SetPen(DARKRED);
	pWind->SetFont(18, BOLD, BY_NAME, "Arial");
	pWind->DrawString(10, WindHeight - StatusBarHeight + 10, msg); // You may need to change these coordinates later
																   // to be able to write multi-line
}

void GUI::PrintTimestep(int time) const
{
	pWind->SetPen(WHITE);
	pWind->SetBrush(WHITE);
	pWind->DrawCircle(WindWidth / 2, YHalfDrawingArea, 30);
	if (time < 10)
	{
		DrawString(WindWidth / 2 - 4, YHalfDrawingArea - 8, to_string(time));
	}
	else
		DrawString(WindWidth / 2 - 8, YHalfDrawingArea - 8, to_string(time));
}

void GUI::PrintRegions(string data[REGION_COUNT], string dataMotor[REGION_COUNT], string dataAssignedMotors[REGION_COUNT], int servedOrders[REGION_COUNT][TYPE_COUNT] ) const
{
	ClearStatusBar(); //First clear the status bar
	pWind->SetPen(PALEVIOLETRED, 3);
	pWind->DrawLine(0, WindHeight - StatusBarHeight / 2 - 10, WindWidth, WindHeight - StatusBarHeight / 2 - 10);
	pWind->DrawLine(WindWidth / 2, WindHeight - StatusBarHeight, WindWidth / 2, WindHeight);

	for (int reg = 0; reg < REGION_COUNT; reg++)
	{
		switch (reg)
		{
		case 0:
			pWind->SetPen(TOMATO);
			pWind->SetFont(20, BOLD, BY_NAME, "Arial");
			pWind->DrawString(200, WindHeight - StatusBarHeight + 5, "A");
			pWind->SetPen(OLIVEDRAB);
			pWind->SetFont(16, PLAIN, BY_NAME, "Arial");
			pWind->DrawString(10, WindHeight - StatusBarHeight + 20, data[reg]);
			pWind->DrawString(10, WindHeight - StatusBarHeight + 35, dataMotor[reg]);
			pWind->DrawString(10, WindHeight - StatusBarHeight + 50, dataAssignedMotors[reg]);
			pWind->DrawString(10, WindHeight - StatusBarHeight + 65, "Total Served Orders: ");
			// pWind->DrawInteger(130, WindHeight - StatusBarHeight + 65, servedOrders[reg]);
			break;

		case 1:
			pWind->SetPen(TOMATO);
			pWind->SetFont(20, BOLD, BY_NAME, "Arial");
			pWind->DrawString(WindWidth / 2 + 200, WindHeight - StatusBarHeight + 5, "B");
			pWind->SetPen(DIMGREY);
			pWind->SetFont(16, PLAIN, BY_NAME, "Arial");
			pWind->DrawString(WindWidth / 2 + 10, WindHeight - StatusBarHeight + 20, data[reg]);
			pWind->DrawString(WindWidth / 2 + 10, WindHeight - StatusBarHeight + 35, dataMotor[reg]);
			pWind->DrawString(WindWidth / 2 + 10, WindHeight - StatusBarHeight + 50, dataAssignedMotors[reg]);
			pWind->DrawString(WindWidth / 2 + 10, WindHeight - StatusBarHeight + 65, "Total Served Orders: ");
			// pWind->DrawInteger(WindWidth / 2 + 130, WindHeight - StatusBarHeight + 65, servedOrders[reg]);
			break;

		case 2:
			pWind->SetPen(TOMATO);
			pWind->SetFont(20, BOLD, BY_NAME, "Arial");
			pWind->DrawString(WindWidth / 2 + 200, WindHeight - StatusBarHeight / 2 - 5, "C");
			pWind->SetPen(GOLDENROD);
			pWind->SetFont(16, PLAIN, BY_NAME, "Arial");
			pWind->DrawString(WindWidth / 2 + 10, WindHeight - StatusBarHeight / 2 + 10, data[reg]);
			pWind->DrawString(WindWidth / 2 + 10, WindHeight - StatusBarHeight / 2 + 25, dataMotor[reg]);
			pWind->DrawString(WindWidth / 2 + 10, WindHeight - StatusBarHeight / 2 + 40, dataAssignedMotors[reg]);
			pWind->DrawString(WindWidth / 2 + 10, WindHeight - StatusBarHeight / 2 + 55, "Total Served Orders: ");
			// pWind->DrawInteger(WindWidth / 2 + 130, WindHeight - StatusBarHeight / 2 + 55, servedOrders[reg]);
			break;

		case 3:
			pWind->SetPen(TOMATO);
			pWind->SetFont(20, BOLD, BY_NAME, "Arial");
			pWind->DrawString(200, WindHeight - StatusBarHeight / 2 - 5, "D");
			pWind->SetPen(DARKCYAN);
			pWind->SetFont(16, PLAIN, BY_NAME, "Arial");
			pWind->DrawString(10, WindHeight - StatusBarHeight / 2 + 10, data[reg]);
			pWind->DrawString(10, WindHeight - StatusBarHeight / 2 + 25, dataMotor[reg]);
			pWind->DrawString(10, WindHeight - StatusBarHeight / 2 + 40, dataAssignedMotors[reg]);
			pWind->DrawString(10, WindHeight - StatusBarHeight / 2 + 55, "Total Served Orders: ");
			// pWind->DrawInteger(130, WindHeight - StatusBarHeight / 2 + 55, servedOrders[reg]);
			break;

		default:
			return;
		}
	}
//<<<<<<< HEAD
	
//=======
	/*for (int reg = 0; reg < REGION_COUNT; reg++)
	{
		int factor = reg * 20;
		pWind->DrawString(10, WindHeight - StatusBarHeight + factor + 5, data[reg]);
	*/
//<<<<<<< HEAD
//>>>>>>> 502e7f286fe068678b80f84eb497505c03a640b9
//=======
//>>>>>>> 502e7f286fe068678b80f84eb497505c03a640b9
}

//////////////////////////////////////////////////////////////////////////////////////////
void GUI::DrawString(const int iX, const int iY, const string Text) const
{
	pWind->SetPen(DARKRED);
	pWind->SetFont(18, BOLD, BY_NAME, "Arial");
	pWind->DrawString(iX, iY, Text);
}

//////////////////////////////////////////////////////////////////////////////////////////
void GUI::ClearStatusBar() const
{
	pWind->SetPen(WHITE, 3);
	pWind->SetBrush(WHITE);
	pWind->DrawRectangle(0, WindHeight - StatusBarHeight, WindWidth, WindHeight);

	pWind->SetPen(BROWN, 3);
	pWind->DrawLine(0, WindHeight - StatusBarHeight, WindWidth, WindHeight - StatusBarHeight);
}
///////////////////////////////////////////////////////////////////////////////////
void GUI::ClearDrawingArea() const
{
	// Clearing the Drawing area
	pWind->SetPen(WHITE, 3);
	pWind->SetBrush(WHITE);
	pWind->DrawRectangle(0, MenuBarHeight, WindWidth, WindHeight - StatusBarHeight);
}
///////////////////////////////////////////////////////////////////////////////////
void GUI::DrawRestArea() const
{
	int L = RestWidth / 2;

	// 1- Drawing the brown square of the Rest
	pWind->SetPen(GOLD);
	pWind->SetBrush(GOLD);
	pWind->DrawCircle((WindWidth / 2), YHalfDrawingArea, (WindWidth / 2) - RestStartX);

	// 2- Drawing the 2 brown crossed lines (for making 4 regions)
	pWind->SetPen(PALEVIOLETRED, 3);
	pWind->DrawLine(0, YHalfDrawingArea, WindWidth, YHalfDrawingArea);
	pWind->DrawLine(WindWidth / 2, 0, WindWidth / 2, WindHeight - StatusBarHeight);
	//image img("rest_del.jpg");
	//pWind->DrawImage(img, 0, 0);

	pWind->SetPen(WHITE);
	pWind->SetBrush(WHITE);
	pWind->DrawCircle(WindWidth / 2, YHalfDrawingArea, 30);
	// 3- Drawing the 2 white crossed lines (inside the Rest)
	/*pWind->SetPen(WHITE);
	pWind->DrawLine(WindWidth/2, YHalfDrawingArea - RestWidth/2, WindWidth/2, YHalfDrawingArea + RestWidth/2);
	pWind->DrawLine(WindWidth/2 - RestWidth/2, YHalfDrawingArea, WindWidth/2 + RestWidth/2, YHalfDrawingArea);
	*/

	// 4- Drawing the 4 white squares inside the Rest (one fo  r each tower)
	/*pWind->SetPen(DODGERBLUE);
	pWind->SetBrush(DODGERBLUE);
	pWind->DrawEllipse(RestStartX + L/4, RestStartY + L/4 , RestStartX + 2*L/2 , RestStartY + 2*L/4 );
	pWind->DrawEllipse(RestStartX + L/4, RestEndY - L/4, RestStartX + 2*L/2, RestEndY - 2*L/4);
	pWind->DrawEllipse(RestEndX - 2*L/3, RestStartY + L/3, RestEndX - L/3, RestStartY + 2*L/3);
	pWind->DrawEllipse(RestEndX - 2*L/3, RestEndY - L/3, RestEndX - L/3, RestEndY - 2*L/3);
	*/
	// 5- Writing the letter of each region (A, B, C, D)
	pWind->SetPen(SLATEBLUE);
	pWind->SetFont(40, ITALICIZED, ROMAN, "Arial");
	pWind->DrawString(RestStartX + (int)(0.44 * L), RestStartY + 5 * L / 12, "A");
	pWind->DrawString(RestStartX + (int)(0.44 * L), YHalfDrawingArea + 5 * L / 20, "D");
	pWind->DrawString(RestStartX + L + (int)(0.36 * L), RestStartY + 5 * L / 12, "B");
	pWind->DrawString(RestStartX + L + (int)(0.36 * L), YHalfDrawingArea + 5 * L / 20, "C");
}
//////////////////////////////////////////////////////////////////////////////////////////
void GUI::DrawSingleOrder(Order *pO, int RegionCount, bool deletes) const // It is a private function
{
	color clr = OrdersClrs[pO->GetType()];
	REGION Region = pO->GetRegion();

	if (RegionCount > MaxRegionOrderCount)
		return; //no more orders can be drawn in this region

	int DrawDistance = RegionCount;
	int YPos = 1;
	if (RegionCount >= MaxHorizOrders) //max no. of orders to draw in one line
	{
		DrawDistance = (RegionCount - 1) % MaxHorizOrders + 1;
		YPos = (RegionCount - 1) / MaxHorizOrders + 1;
	}

	int x, y, refX, refY;
	//First calculate x,y position of the order on the output screen
	//It depends on the region and the order distance

	switch (Region)
	{
	case A_REGION:
		refX = (WindWidth / 2 - RestWidth / 2);
		refY = YHalfDrawingArea - OrderHeight;				 //
		x = refX - DrawDistance * OrderWidth - DrawDistance; //(Distance)
		y = refY - YPos * OrderHeight - YPos;				 // YPos
		break;
	case B_REGION:
		refX = (WindWidth / 2 + RestWidth / 2 + 15);
		refY = YHalfDrawingArea - OrderHeight;					   //
		x = refX + (DrawDistance - 1) * OrderWidth + DrawDistance; //(Distance)
		y = refY - YPos * OrderHeight - YPos;					   // YPos
		break;
	case C_REGION:
		refX = (WindWidth / 2 + RestWidth / 2 + 15);
		refY = YHalfDrawingArea + OrderHeight;					   //
		x = refX + (DrawDistance - 1) * OrderWidth + DrawDistance; //(Distance)
		y = refY + (YPos - 1) * OrderHeight + YPos;				   // YPos
		break;
	case D_REGION:
		refX = (WindWidth / 2 - RestWidth / 2);
		refY = YHalfDrawingArea + OrderHeight;				 //
		x = refX - DrawDistance * OrderWidth - DrawDistance; //(Distance)
		y = refY + (YPos - 1) * OrderHeight + YPos;			 // YPos
		break;
	default:
		break;
	}
	if (deletes)
	{
		pWind->SetPen(clr);
		pWind->SetBrush(clr);
		pWind->SetFont(20, BOLD, MODERN);
		pWind->DrawInteger(x, y, pO->GetID());
	}
	else
		Animate(x, y, pO->GetID(), clr, Region);
}

void GUI::Animate(int x, int y, int id, color colr, REGION reg) const
{
	if (id % 3 == 1)
	{
		pWind->SetPen(WHITE);
		pWind->SetBrush(WHITE);
		pWind->DrawCircle(WindWidth / 2, YHalfDrawingArea, 25);
		pWind->SetPen(DARKRED);
		pWind->SetBrush(DARKRED);
		pWind->DrawCircle(WindWidth / 2 - 10, YHalfDrawingArea, 2);
	}
	else if (id % 3 == 2)
	{
		pWind->SetPen(WHITE);
		pWind->SetBrush(WHITE);
		pWind->DrawCircle(WindWidth / 2, YHalfDrawingArea, 25);
		pWind->SetPen(DARKRED);
		pWind->SetBrush(DARKRED);
		pWind->DrawCircle(WindWidth / 2, YHalfDrawingArea, 2);
	}
	else if (id % 3 == 0)
	{
		pWind->SetPen(WHITE);
		pWind->SetBrush(WHITE);
		pWind->DrawCircle(WindWidth / 2, YHalfDrawingArea, 25);
		pWind->SetPen(DARKRED);
		pWind->SetBrush(DARKRED);
		pWind->DrawCircle(WindWidth / 2 + 10, YHalfDrawingArea, 2);
	}
	// Drawing the Order
	for (int i = 120; i > 0; i-=2)
	{
		pWind->SetPen(colr);
		pWind->SetBrush(colr);
		pWind->SetFont(20, BOLD, MODERN);
		if (reg == A_REGION || reg == D_REGION)
		{
			pWind->DrawInteger(x - i, y, id);
			Sleep(1);
			pWind->SetPen(WHITE);
			pWind->SetBrush(WHITE);
			pWind->DrawRectangle(x - i, y, x - i + OrderWidth - 10, y + 15);
			//pWind->DrawCircle(x - i + 10, y + 7, 20);
		}
		else
		{
			pWind->DrawInteger(x + i, y, id);
			Sleep(1);
			pWind->SetPen(WHITE);
			pWind->SetBrush(WHITE);
			pWind->DrawRectangle(x + i, y, x + i + OrderWidth - 10, y + 15);
			//pWind->DrawCircle(x + i + 10, y + 7, 20);
		}
	}
	pWind->SetPen(colr);
	pWind->SetBrush(colr);
	pWind->SetFont(20, BOLD, MODERN);
	pWind->DrawInteger(x, y, id);
}

//////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////
/* A function to draw a list of orders and ensure there is no overflow in the drawing*/
// To use this function, you must prepare its input parameters as specified
// [Input Parameters]:
//    orders [ ] : array of Order pointers (ALL orders from all regions in one array)
//    TotalOrders : the size of the array (total no. of orders)
void GUI::DrawOrders(bool delet) const
{
	//Prepare counter for each region
	int RegionsCounts[REGION_COUNT] = {0}; //initlaize all counters to zero

	for (int i = 0; i < OrderCount; i++)
	{
		int orderRegion = OrdListForDrawing[i]->GetRegion();
		RegionsCounts[orderRegion]++;
		DrawSingleOrder(OrdListForDrawing[i], RegionsCounts[orderRegion], delet);
	}
}

void GUI::UpdateInterface(bool del) const
{
	ClearDrawingArea();
	DrawRestArea();
	DrawOrders(del);
}

/*
	AddOrderForDrawing: Adds a new order to the drawing list
*/
void GUI::AddOrderForDrawing(Order *ptr)
{
	if (OrderCount < MaxPossibleOrdCnt)
		OrdListForDrawing[OrderCount++] = ptr;

	// Note that this function doesn't allocate any Order objects
	// It only makes the first free pointer in the array
	// points to the same Order pointed to by "ptr"
}

void GUI::ResetDrawingList()
{
	OrderCount = 0; //resets the orders count to be ready for next timestep updates
}

PROGRAM_MODE GUI::getGUIMode() const
{
	PROGRAM_MODE Mode;
	do
	{
		PrintMessage("Please select GUI mode: (1) Interactive, (2) StepByStep, (3) Silent");
		string S = GetString();
		Mode = (PROGRAM_MODE)(atoi(S.c_str()) - 1);
	} while (Mode < 0 || Mode >= MODE_COUNT);

	return Mode;
}
