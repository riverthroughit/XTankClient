#pragma once

#include "ECS/System.h"

class EntitySpawnSystem :public System {

public:
	virtual void Tick(float dt) override;
};