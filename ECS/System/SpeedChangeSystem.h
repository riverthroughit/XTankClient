#pragma once

#include "ECS/System.h"

class SpeedChangeSystem :public System<SpeedChangeSystem> {

public:
	virtual void Tick(float dt) override;
};