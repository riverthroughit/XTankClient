#pragma once

#include "Config.h"
#include "Util/TickUtil.h"

struct FrameComponent {

	//单例组件

	//用于管理客户端逻辑帧id
	TickUtil clientTick{ LOCKSTEP_TICK };

	//用于管理服务器逻辑帧id
	int serverFrameId{ -1 };


};