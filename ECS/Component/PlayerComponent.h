#pragma once

#include "ECS/Types.h"

struct PlayerComponent {
	//���id
	unsigned int playerId{};
	//�ٿص�entity
	Entity charId{};
	//����ֵ
	int hp{};
	//����
	unsigned int score{};
	//(�ٿص�entity��)ʣ�ิ��ʱ��
	int respawnTime{};
};