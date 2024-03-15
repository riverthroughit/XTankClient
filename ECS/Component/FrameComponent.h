#pragma once

#include <chrono>
#include "Config.h"

struct FrameComponent {

	//单例组件


	using clock = std::chrono::high_resolution_clock;
	using duration = std::chrono::duration<float, std::milli>;

	//周期
	float tickTime{ LOCKSTEP_TICK };
	//当前帧id
	int frameId{-1};
	//当前帧内的时间
	float dt{ LOCKSTEP_TICK };
	//当前帧进度
	float percent{};
	//是否经过一个逻辑帧周期
	bool isNeedTick{};

	clock::time_point preTime{};
	clock::time_point curTime{};

	bool isStart{};

};