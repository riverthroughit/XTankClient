#pragma once

#include "ECS/System.h"
#include <chrono>


class FrameSystem :public System<FrameSystem> {

public:
	virtual void Tick(float = 0) override;

	virtual void Init()override;

	//增加一次帧id
	void AddServerFrameId();
};