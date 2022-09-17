#pragma once

#include "Unit.hpp"

class Player : public Unit {
public:
	int health;
	int timeOutFrames;
	Player();
	Player(int x, int y);
	~Player();
};