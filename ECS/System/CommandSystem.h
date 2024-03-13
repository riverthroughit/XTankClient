#pragma once

#include "ECS/System.h"

class CommandSystem :public System<CommandSystem> {
public:
	virtual void Tick(float dt) override;
};