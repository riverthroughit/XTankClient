#pragma once

#include "ECS/System.h"

class FrameSystem :public System {

public:
	virtual void Tick(float dt) override;

};