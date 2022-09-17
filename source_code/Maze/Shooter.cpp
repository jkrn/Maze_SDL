#include "Shooter.hpp"

Shooter::Shooter() : Shooter(0, 0) {}

Shooter::Shooter(int x, int y) : Unit(x, y){
	isPlayer = false;
}

Shooter::~Shooter() {}

void Shooter::move(int dx, int dy) {}