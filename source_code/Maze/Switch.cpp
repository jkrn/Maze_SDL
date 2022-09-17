#include "Switch.hpp"

Switch::Switch() : Switch(0, 0) {}

Switch::Switch(int x, int y) : Unit(x, y){
	isPlayer = false;
}

Switch::~Switch() {}

void Switch::move(int dx, int dy) {}