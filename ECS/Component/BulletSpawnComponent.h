#pragma once
#include "Math/Vec2.h"

struct BulletSpawnComponent
{
	//�ӵ��ٶȷ���
	Vec2Fixed spawnDirec;
	//�Ƿ��ܷ����ӵ�
	bool canFire{};
	//��ȴcd
	int reFireTime{};
};