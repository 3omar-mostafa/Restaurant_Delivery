#ifndef __GUI_H_
#define __GUI_H_

#include "..\CMUgraphicsLib\CMUgraphics.h"
#include "..\Defs.h"

#include "..\Restaurant\Order.h"

#include <string>
using namespace std;

class GUI
{

  private:
	window *pWind;
	color OrdersClrs[TYPE_COUNT];

	// Some Static Constant Data Members: ---------------------

	static const int
		WindWidth = 1200,
		WindHeight = 650,	  //Window width and height
		StatusBarHeight = 200, //Status Bar Height
		MenuBarHeight = 20,	//Menu Bar Height (distance from top of window to bottom line of menu bar)
		MenuItemWidth = 80,	//Width of each item in menu bar menu

		DrawingAreaHeight = WindHeight - MenuBarHeight - StatusBarHeight,
		YHalfDrawingArea = MenuBarHeight + DrawingAreaHeight / 2, //The y coordinate of half the Drawing area

		RestStartX = (int)(WindWidth * 0.4), //The x coordinate of the upper left corner of the Rest
		RestEndX = (int)(WindWidth * 0.6),   //The x coordinate of the lower right corner of the Rest

		RestWidth = RestEndX - RestStartX, //The width of the Rest (Note: the Rest is a Square)

		RestStartY = YHalfDrawingArea - RestWidth / 2, //The y coordinate of the upper left corner of the Rest
		RestEndY = YHalfDrawingArea + RestWidth / 2,   //The y coordinate of the lower right corner of the Rest

		FontSize = 20,			   //font size used to draw orders ID on Interface
		OrderWidth = 2 * FontSize, //width of the order to be drawn on GUI
		OrderHeight = FontSize,	//height of the order to be drawn on GUI

		MaxHorizOrders = ((WindWidth - RestWidth) / 2) / (OrderWidth + 1), //The max no. of orders the can be drwan in on Horizontal line in a region
		MaxVerticalOrders = (DrawingAreaHeight / 2) / (OrderHeight + 1),   //The max no. of orders the can be drwan in on Horizontal line in a region

		//Max no of orders that can be drawn in a single region
		MaxRegionOrderCount = MaxHorizOrders * MaxVerticalOrders;

	/////////
	////////
	////////

	int OrderCount; //the total number of orders to be drawn

	Order *OrdListForDrawing[MaxPossibleOrdCnt]; // This Array of Pointers is used for drawing elements in the GUI
	//NOTES:
	//Orders are assumed to be sorted by arrival time
	// At every time step, you should update those pointers
	// to point to the current waiting orders only

	//
	// TODO: Add more members if needed
	//

	void DrawSingleOrder(Order* pO, int RegionCount, bool deletes = 0,int time=0) const;		//draws ONE order 
	void DrawOrders(bool delet = 0,int time=0) const;		//draws ALL orders in OrdListForDrawing
	void Animate(int x, int y, int id, color colr, REGION reg,int time=0) const;
	

	void DrawString(const int iX, const int iY, const string Text) const; // prints a message in the passed coordinates
	void DrawRestArea() const;	// draws the restaurant area
	void DrawStars(int time, bool flip = 0) const;
	

	void ClearStatusBar() const;   // clears the status bar
	void ClearDrawingArea(int time) const; // clears the Drawing area from all drawings

  public:
	GUI();
	~GUI();

	// Input Functions  ---------------------------
	void waitForClick() const; // waits a user click from the user
	string GetString() const;  // reads a string (keyboard input) from the user

	// Output Functions  ---------------------------
	void PrintMessage(string msg) const; // prints a message in the status bar
	void PrintTimestep(int time) const;
	void PrintRegions(string data[REGION_COUNT], string dataMotor[REGION_COUNT], string dataAssignedMotors[REGION_COUNT], string servedOrders[REGION_COUNT]) const;
	

	void UpdateInterface(bool del = 0,int time=0) const;
	void UpdateInterface(color newColor) const;
	void AddOrderForDrawing(Order *pOrd); //Adds a new order to the drawing list
	void ResetDrawingList();			  //resets drawing list (should be called every timestep after drawing)

	PROGRAM_MODE getGUIMode() const; //returns the mode of the program
};

#endif