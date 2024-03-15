#pragma once
#include "Math/Vec2.h"


struct SpeedComponent {
	//速度方向
	Vec2Fixed direc;
	//速度大小
	FixedPoint speed;

	//加速度方向
	Vec2Fixed accDirec;
	//加速度大小 0到1
	FixedPoint accSpeed;
};