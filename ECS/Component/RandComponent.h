#pragma once
#include <cmath>

struct RandComponent {

	//单例组件

	unsigned int seed{};

public:
	void SetSeed(unsigned int s) {
		seed = s;
		std::srand(seed);
	}

	//概率函数 numerator/denominator 的概率为真
	bool IsTrueByProb(int numerator, int denominator) {
		return numerator > (rand() % denominator);
	}
};