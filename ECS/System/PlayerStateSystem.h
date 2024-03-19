#pragma once

#include "ECS/System.h"

class PlayerStateSystem :public System<PlayerStateSystem> {

public:
	virtual void Tick(float dt) override;


private:

	//子弹是否击中其他entity
	bool IsBulletHit(Entity player);

	//击中其他坦克的数量
	int GetHitTankNum(Entity player);

	//是否被击杀
	bool IsHitByOther(Entity player);

	//是否死亡
	bool IsDead(Entity player);

	//是否正在复活
	bool IsReviving(Entity player);
};