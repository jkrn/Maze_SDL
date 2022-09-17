#include "Boss.hpp"

Boss::Boss() : Boss(0, 0) {}

Boss::Boss(int x, int y) : Unit(x, y){
	isPlayer = false;
}

Boss::~Boss() {}

void Boss::move(int dx, int dy) {}