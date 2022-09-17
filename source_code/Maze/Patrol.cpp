#include "Patrol.hpp"
#include "Constants.hpp"

Patrol::Patrol(double cx, double cy, int dir) :
	Projectile(cx, cy - patrolOffset, (double)dir, 0, false),
	cx(cx),
	cy(cy),
	dir(dir)
{}

Patrol::~Patrol() {}

void Patrol::changeDirection() {
	// startPos
	startPosX = getX();
	startPosY = getY();
	// w, steps
	w = 0;
	steps = 0;
	// dx, dy, horizontalMove
	if (horizontalMove && dir*dx > 0) {
		horizontalMove = false;
		dx = 0;
		dy = 1;
	}
	else if (!horizontalMove && dy > 0) {
		horizontalMove = true;
		dx = -dir;
		dy = 0;
	}
	else if (horizontalMove && dir*dx < 0) {
		horizontalMove = false;
		dx = 0;
		dy = -1;
	}
	else if (!horizontalMove && dy < 0) {
		horizontalMove = true;
		dx = dir;
		dy = 0;
	}
}