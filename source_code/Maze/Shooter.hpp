#pragma once

#include "Unit.hpp"

class Shooter : public Unit {
public:
	Shooter();
	Shooter(int x, int y);
	~Shooter();
	void move(int dx, int dy);
};