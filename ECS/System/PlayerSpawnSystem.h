#pragma once

#include "ECS/System.h"

class PlayerSpawnSystem :public System {

public:
	virtual void Tick(float dt)override;

private:

};