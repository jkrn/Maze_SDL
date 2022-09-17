#include <algorithm>

#include "RandomGenerator.hpp"

RandomGenerator::RandomGenerator(int min, int max) : index(0), range(max - min + 1) {
	for (int currVal = min; currVal <= max; currVal++) {
		values.push_back(currVal);
	}
	std::random_shuffle(std::begin(values), std::end(values));
}

RandomGenerator::~RandomGenerator() {}

int RandomGenerator::getNextValue() {
	int nextValue = values.at(index);
	index++;
	if (index == range) {
		std::random_shuffle(std::begin(values), std::end(values));
		index = 0;
	}
	return nextValue;
}