#pragma once
#include "ECS/System.h"

class MoveSystem :public System {

public:
	virtual void Tick(float dt)override;

};