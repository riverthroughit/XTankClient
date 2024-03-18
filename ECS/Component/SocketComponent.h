#pragma once

#include "TypeConfig.h"
#include <array>
#include <vector>
#include <deque>

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

	//当前缓存的命令
	std::deque<PlayersCommand> playersCmdsBuffer;

	//当前收到的服务器帧id
	int curServerFrameId{-1};

	//本地玩家是否为 中途加入 且正在第一次追帧
	bool isCutInChasing{};

};