#pragma once

#include "ECS/System.h"

class AttachSystem :public System<AttachSystem> {

public:
	virtual void Tick(float dt) override;

};