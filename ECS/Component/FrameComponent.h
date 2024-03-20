#pragma once

#include "Config.h"
#include "Util/TickUtil.h"

struct FrameComponent {

	//单例组件

	//用于管理客户端逻辑帧id
	TickUtil clientTick{ LOCKSTEP_TICK };

	//服务器当前逻辑帧id
	int serverFrameId{ -1 };

};