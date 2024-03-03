#pragma once

#include "ECS/System.h"

class PlayerStateSystem :public System {

public:
	virtual void Tick(float dt) override;

};