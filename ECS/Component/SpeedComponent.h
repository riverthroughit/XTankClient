#pragma once
#include "Math/Vec2.h"


struct SpeedComponent {
	//�ٶȷ���
	Vec2Fixed direc;
	//�ٶȴ�С
	FixedPoint speed;

	//���ٶȷ���
	Vec2Fixed accDirec;
	//���ٶȴ�С 0��1
	FixedPoint accSpeed;
};