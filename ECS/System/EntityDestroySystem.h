#pragma once

#include "ECS/System.h"

class EntityDestroySystem :public System {

public:
	virtual void Tick(float dt) override;

};