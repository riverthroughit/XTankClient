#pragma once

#include "ECS/System.h"

class BulletHitSystem :public System<BulletHitSystem> {

public:
	virtual void Tick(float dt) override;

private:

};