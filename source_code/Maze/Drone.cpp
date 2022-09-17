#include "Drone.hpp"

Drone::Drone() : Drone(0, 0) {}

Drone::Drone(int x, int y) : Unit(x, y), cx(x), cy(y){
	isPlayer = false;
}

Drone::~Drone() {}