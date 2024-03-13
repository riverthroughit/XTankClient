#pragma once
#include "ECS/System.h"

class MoveSystem :public System<MoveSystem> {

public:
	virtual void Tick(float dt)override;

};