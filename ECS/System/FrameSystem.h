#pragma once

#include "ECS/System.h"

class FrameSystem :public System {

public:
	virtual void Tick(float dt) override;

	//回滚时的更新
	void TickInRollback();
};