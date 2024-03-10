#pragma once

#include "Config.h"

struct FrameComponent {

	//单例组件


	//当前帧id
	int frameId{-1};
	//当前帧内的时间
	float frameTime{ LOCKSTEP_TICK };
	//当前帧进度
	float percent{};
	//是否经过一个逻辑帧周期
	bool isNeedTick{};
};