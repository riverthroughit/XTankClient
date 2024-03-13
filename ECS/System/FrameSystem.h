#pragma once

#include "ECS/System.h"
#include <chrono>


class FrameSystem :public System<FrameSystem> {


	using clock = std::chrono::high_resolution_clock;
	using duration = std::chrono::duration<float, std::milli>;

public:
	virtual void Tick(float = 0) override;

	virtual void Init()override;

	//回滚时的更新
	void TickInRollback();

	void Reset();

	void SetTickTime(float val);

	bool IsNeedTick();

	//设置一次延迟
	void SetDelay(float val);

	float GetDt();
};