#pragma once
#include "Math/Vec2.h"

struct BulletSpawnComponent
{
	//子弹速度方向
	Vec2Fixed spawnDirec;
	//是否能发射子弹
	bool canFire{};
	//冷却cd
	int reFireTime{};
};