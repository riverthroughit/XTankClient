#pragma once

#include "ECS/System.h"

class EntityDestroySystem :public System<EntityDestroySystem> {

public:
	virtual void Tick(float dt) override;

};