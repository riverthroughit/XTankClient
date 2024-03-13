#pragma once

#include "ECS/System.h"

class PlayerStateSystem :public System<PlayerStateSystem> {

public:
	virtual void Tick(float dt) override;

};