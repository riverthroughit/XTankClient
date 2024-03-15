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

	//����һ���ӳ�
	void SetDelay(float val);

	float GetDt();

	bool FrameSystem::IsNeedTick();

	//��Ԥ�������е�tick
	void TickInDuplicate();
};