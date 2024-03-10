#pragma once

#include "TypeConfig.h"
#include <array>

struct SocketComponent{

	//单例组件

	//本地用户的房间内id
	unsigned int localPlayerId{};

	//玩家数量
	int playerNum{};

	//是否有新来的命令
	bool hasNewCmdMsg{};

	PlayersCommand curPlayersCmd{};
};