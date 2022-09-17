#pragma once

#include "Unit.hpp"

class Switch : public Unit {
public:
	Switch();
	Switch(int x, int y);
	~Switch();
	void move(int dx, int dy);
};