#pragma once

#include "ECS/System.h"

class PlayerStateSystem :public System<PlayerStateSystem> {

public:
	virtual void Tick(float dt) override;


private:

	//�ӵ��Ƿ��������entity
	bool IsBulletHit(Entity player);

	//��������̹�˵�����
	int GetHitTankNum(Entity player);

	//�Ƿ񱻻�ɱ
	bool IsHitByOther(Entity player);

	//�Ƿ�����
	bool IsDead(Entity player);

	//�Ƿ����ڸ���
	bool IsReviving(Entity player);
};