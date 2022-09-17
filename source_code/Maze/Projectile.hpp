#pragma once

class Projectile {
public:
	int steps;
	double startPosX;
	double startPosY;
	double dx;
	double dy;
	double w;
	bool angled;
	bool horizontalMove;
	Projectile();
	Projectile(double x, double y, double dx, double dy, bool angled);
	~Projectile();
	double getX();
	double getY();
};