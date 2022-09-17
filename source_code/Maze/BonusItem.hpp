#pragma once

#include "Point.hpp"

class BonusItem : public Point {
public:
	int value;
	BonusItem(int x, int y, int value);
};