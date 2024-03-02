#pragma once

#include "ECS/System.h"

class SpeedChangeSystem :public System {

public:
	virtual void Tick(float dt) override;
};