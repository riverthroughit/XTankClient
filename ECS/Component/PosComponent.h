#pragma once

#include"Math/Vec2.h"


struct PosComponent {
	Vec2Fixed pos;
	Vec2Fixed direc;

	//���ڲ�ֵ
	Vec2Fixed prePos;
	Vec2Fixed preDirec;

};
