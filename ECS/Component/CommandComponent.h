#pragma once

#include "Util/Macro.h"
#include <map>
#include "TypeConfig.h"


struct CommandComponent {
	
	//命令产生时的逻辑帧id
	unsigned int frameId{};
	//命令类型
	BUTTON::Type cmd{ BUTTON::NONE };

	bool HasSpeedChangeCmd() {
		return cmd == BUTTON::UP || cmd == BUTTON::DOWN 
			|| cmd == BUTTON::LEFT || cmd == BUTTON::RIGHT
			|| cmd == BUTTON::NONE;
	}
};