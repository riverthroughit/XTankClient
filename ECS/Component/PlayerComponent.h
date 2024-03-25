#pragma once

#include "ECS/Types.h"
#include "TypeConfig.h"

struct PlayerComponent {
	//��ҷ�����id
	unsigned int localId{};
	//�ٿص�entity
	Entity pawnId{};
	//���״̬
	PLAYER_STATUS::Type status{};
	//����ֵ
	int hp{};
	//����
	unsigned int score{};
	//(�ٿص�entity��)ʣ�ิ��ʱ��
	int respawnTime{};
};