#pragma once

#include "Unit.hpp"

class Drone : public Unit {
public:
	int cx;
	int cy;
	Drone();
	Drone(int x, int y);
	~Drone();
};