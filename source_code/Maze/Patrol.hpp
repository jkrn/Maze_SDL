#pragma once

#include "Projectile.hpp"

class Patrol : public Projectile {
public:
	double cx;
	double cy;
	int dir;
	Patrol(double cx, double cy, int dir);
	~Patrol();
	void changeDirection();
};