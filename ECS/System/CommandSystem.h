#pragma once

#include "ECS/System.h"

class CommandSystem :public System {
public:
	virtual void Tick(float dt) override;
};