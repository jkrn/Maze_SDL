#pragma once

#define NORTH_VAL	1
#define SOUTH_VAL	2
#define EAST_VAL	4
#define WEST_VAL	8

enum DIR { NORTH, SOUTH, EAST, WEST };

class Direct {
public:
	int bit;
	int dx;
	int dy;
	int oppBit;
	Direct(DIR dir);
	void setValues(int bit, int dx, int dy, int oppBit);
};