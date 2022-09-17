#pragma once

#include "Unit.hpp"

class Boss : public Unit {
public:
	Boss();
	Boss(int x, int y);
	~Boss();
	void move(int dx, int dy);
};