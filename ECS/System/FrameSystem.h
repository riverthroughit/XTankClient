#pragma once

#include "ECS/System.h"
#include <chrono>


class FrameSystem :public System<FrameSystem> {


	using clock = std::chrono::high_resolution_clock;
	using duration = std::chrono::duration<float, std::milli>;

public:
	virtual void Tick(float = 0) override;

	virtual void Init()override;

	void Reset();

	void SetTickTime(float val);

	//设置一次延迟
	void SetDelay(float val);

	//设置帧id
	void SetFrameId(int id);

	float GetDt();

	bool FrameSystem::IsNeedTick();

	//增加一次帧id
	void AddFrameId();
};