#pragma once

#include "ECS/System.h"

class FireSystem :public System<FireSystem> {

public:
	virtual void Tick(float dt) override;
};