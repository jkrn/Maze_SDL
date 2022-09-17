#include "Projectile.hpp"

Projectile::Projectile() : Projectile(0, 0, 0, 0, false) {}

Projectile::Projectile(double x, double y, double dx, double dy, bool angled) :
	steps(0),
	startPosX(x),
	startPosY(y),
	dx(dx),
	dy(dy),
	w(0),
	angled(angled)
{
	if (!angled && dx != 0) {
		horizontalMove = true;
	}
	else {
		horizontalMove = false;
	}
}

Projectile::~Projectile() {}

double Projectile::getX() {
	if (angled || horizontalMove) {
		return startPosX + dx * w;
	}
	return startPosX;
}

double Projectile::getY() {
	if (angled || !horizontalMove) {
		return startPosY + dy * w;
	}
	return startPosY;
}