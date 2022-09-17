#pragma once

#include <vector>

class RandomGenerator {
public:
	RandomGenerator(int min, int max);
	~RandomGenerator();
	int getNextValue();
private:
	const int range;
	int index;
	std::vector<int> values;
};