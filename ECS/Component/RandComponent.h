#pragma once
#include <cmath>

struct RandComponent {

	//�������

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