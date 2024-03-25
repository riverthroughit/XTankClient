#pragma once

#include "ECS/System.h"

class PlayerStateSystem :public System<PlayerStateSystem> {

public:
	virtual void Tick(float dt) override;


private:

	//子弹击中其他entity
	void BulletHit(Entity player);

	//是否被击杀
	void HitByOther(Entity player);

	//是否死亡
	void Dead(Entity player);

	//复活
	void Reviving(Entity player);
};