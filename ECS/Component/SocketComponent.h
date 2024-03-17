#pragma once

#include "TypeConfig.h"
#include <array>
#include <vector>

struct SocketComponent{

	//单例组件

	//本地用户的房间内id
	int localPlayerId{-1};

	//玩家数量
	int playerNum{};

	//是否有新来的命令
	bool hasNewCmdMsg{};

	//服务器发来的命令
	PlayersCommand curPlayersCmd{};

	//中途加入服务器下发的命令
	std::vector<PlayersCommand> chasingCmds;
	//当前追到的帧id
	int chasingFrameId{};

};