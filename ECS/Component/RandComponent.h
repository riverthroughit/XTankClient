#pragma once
#include <cmath>
#include "Util/Macro.h"

struct RandComponent {

	SINGLETON(RandComponent);

	unsigned int seed{};

public:
	void SetSeed(unsigned int s) {
		seed = s;
		std::srand(seed);
	}

	bool IsTrueByProb(int numerator, int denominator) {
		return numerator > (rand() % denominator);
	}
};