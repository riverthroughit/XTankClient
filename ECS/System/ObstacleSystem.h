#pragma once

#include "ECS/System.h"

class ObstacleSystem :public System {

public:
	virtual void Tick(float dt) override;

private:
	//BLOCK���͵��ϰ�������ٶȸ���
	void BlockTypeSpeedTick(Entity entity);
};