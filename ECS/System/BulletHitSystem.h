#pragma once

#include "ECS/System.h"

class BulletHitSystem :public System {

public:
	virtual void Tick(float dt) override;

private:
	void hitTank(Entity bulletId,Entity tankId);
};