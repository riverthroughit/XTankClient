#pragma once

#include "Util/Macro.h"

struct FrameComponent {

	SINGLETON(FrameComponent);

	//当前帧id
	unsigned int frameId{};
	//当前帧内的时间
	float frameTime{};
	//当前帧进度
	float percent{};
	//是否经过一个逻辑帧周期
	bool isNeedTick{};
};