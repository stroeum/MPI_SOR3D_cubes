/*
 *  SizeGrid.h
 *  Created by Jeremy Riousset on 10/25/07.
 */

#ifndef SIZEGRID_H
#define SIZEGRID_H
#include <iostream>
using namespace std;

/**************************************************************************************/
class SizeGrid
{
public:
	int x,y,z;
	SizeGrid();
	SizeGrid(int xx, int yy, int zz);	
	double max();
	bool init(int xx, int yy, int zz);
	~SizeGrid();
}; // Number of steps in the box
/**************************************************************************************/

#endif // SIZEGRID_H
