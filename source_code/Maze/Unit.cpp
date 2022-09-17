#include "Unit.hpp"

Unit::Unit() : Unit(0,0) {}

Unit::Unit(int x, int y) :
	w(0),
	steps(0),
	prevPosX(x),
	prevPosY(y),
	nextPosX(x),
	nextPosY(y),
	dx(0),
	dy(0),
	isMoving(false)
{}

Unit::~Unit() {}

void Unit::move(int dx, int dy) {
	w = 0;
	steps = 0;
	nextPosX = this->prevPosX + dx;
	nextPosY = this->prevPosY + dy;
	this->dx = dx;
	this->dy = dy;
	isMoving = true;
}

void Unit::resetMovement() {
	w = 0;
	steps = 0;
	prevPosX = nextPosX;
	prevPosY = nextPosY;
	dx = 0;
	dy = 0;
	isMoving = false;
}

double Unit::getX() {
	if (isMoving) {
		return prevPosX + dx * w;
	}
	return prevPosX;
}

double Unit::getY() {
	if (isMoving) {
		return prevPosY + dy * w;
	}
	return prevPosY;
}