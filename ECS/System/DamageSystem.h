#pragma once

#include "ECS/System.h"

class DamageSystem :public System<DamageSystem> {

public:
	virtual void Tick(float dt) override;

};