#pragma once
#include <cmath>

struct RandComponent {

	//�������

	unsigned int seed{};

public:

	//���ʺ��� numerator/denominator �ĸ���Ϊ��
	bool IsTrueByProb(int numerator, int denominator) {
		return numerator > (rand() % denominator);
	}
};