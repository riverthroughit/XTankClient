#pragma once

#include "ECS/System.h"

class PlayerStateSystem :public System<PlayerStateSystem> {

public:
	virtual void Tick(float dt) override;


private:

	//�ӵ���������entity
	void BulletHit(Entity player);

	//�Ƿ񱻻�ɱ
	void HitByOther(Entity player);

	//�Ƿ�����
	void Dead(Entity player);

	//����
	void Reviving(Entity player);
};