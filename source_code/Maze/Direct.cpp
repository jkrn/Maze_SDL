#include "Direct.hpp"

Direct::Direct(DIR dir) {
	switch (dir) {
		case NORTH: setValues(NORTH_VAL, 0, -1, SOUTH_VAL); break;
		case SOUTH: setValues(SOUTH_VAL, 0, 1, NORTH_VAL); break;
		case EAST: setValues(EAST_VAL, 1, 0, WEST_VAL); break;
		case WEST: setValues(WEST_VAL, -1, 0, EAST_VAL); break;
	}
}

void Direct::setValues(int bit, int dx, int dy, int oppBit) {
	this->bit = bit;
	this->dx = dx;
	this->dy = dy;
	this->oppBit = oppBit;
}