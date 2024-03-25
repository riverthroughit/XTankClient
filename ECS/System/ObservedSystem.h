#pragma once

#include "ECS/System.h"

class ObservedSystem :public System<ObservedSystem> {

public:
	virtual void Tick(float dt) override;

};