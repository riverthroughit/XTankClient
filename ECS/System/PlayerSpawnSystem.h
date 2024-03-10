#pragma once

#include "ECS/System.h"

class PlayerSpawnSystem :public System {

public:
	virtual void Init() override;
	virtual void Tick(float dt)override;

private:

};