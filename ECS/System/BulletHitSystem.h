#pragma once

#include "ECS/System.h"

class BulletHitSystem :public System<BulletHitSystem> {

public:
	virtual void Tick(float dt) override;

private:

	//销毁子弹 并设置炮台为可发射
	void BulletDestroy(Entity bulletId);

	void HitBlock(Entity bulletId, Entity blockId);

	void HitTank(Entity bulletId,Entity tankId);

	void HitBullet(Entity bulletId1, Entity bulletId2);
};