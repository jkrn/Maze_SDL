#pragma once

class Unit {
public:
	double w;
	int steps;
	int prevPosX;
	int prevPosY;
	int nextPosX;
	int nextPosY;
	int dx;
	int dy;
	bool isMoving;
	bool isPlayer;
	Unit();
	Unit(int x, int y);
	virtual ~Unit() = 0;
	virtual void move(int dx, int dy);
	void resetMovement();
	double getX();
	double getY();
};