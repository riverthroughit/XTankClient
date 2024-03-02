#pragma once

#include "ECS/System.h"

class ObstacleSystem :public System {

public:
	virtual void Tick(float dt) override;

private:
	//BLOCK类型的障碍组件的速度更新
	void BlockTypeSpeedTick(Entity entity);
};