#pragma once

#include "ECS/System.h"
#include <chrono>


class FrameSystem :public System<FrameSystem> {

public:
	virtual void Tick(float = 0) override;

	virtual void Init()override;

	//增加一次服务器帧id
	void AddServerFrameId();

	//更新本地逻辑帧时间
	void TickClientFrame();

	//本地逻辑帧是否可更新
	bool NeedClientTick();

	//本地逻辑帧间隔
	float GetClientTickDt();

	//到下一个本地逻辑帧的时间
	float GetTimeToNextClientFrame();
};