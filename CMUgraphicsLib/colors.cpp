/* 
See "version.h" for version info and copyright information
This file was last modified on 05.16.1999
*/

#include "colors.h"

bool operator==(color a, color b) {

  return ((a.ucRed == b.ucRed) && (a.ucGreen == b.ucGreen) && (a.ucBlue == b.ucBlue));

}

bool operator!=(color a, color b) {

  return !(a == b);
  
}

color::color(std::string hexCode)
{
	int value[3] = { 0 };
	for (int i = 0; i < 3; i++)
	{
		std::string hc = hexCode.substr(i * 2, 2);
		value[i] += (hc[1] >= 'a' && hc[1] <= 'f') ? (hc[1] - 'a' + 10) : (hc[1] - '0');
		value[i] += (hc[0] >= 'a' && hc[0] <= 'f') ? 16 * (hc[0] - 'a' + 10) : 16 * (hc[0] - '0');
	}
	ucRed = value[0], ucGreen = value[1], ucBlue = value[2];
}
