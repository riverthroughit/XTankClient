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

	//是否有当前需要执行的命令
	bool hasCurCmd{};

	//服务器发来的命令
	PlayersCommand curPlayersCmd{};

	//当前缓存的命令
	std::deque<PlayersCommand> playersCmdsBuffer;

	//当前收到的服务器帧id
	int curServerFrameId{-1};

	//上一次发送本地操作时的帧id
	int lastSendFrameId{ -1 };

	//本地玩家是否为 中途加入 且正在第一次追帧
	bool isCutInChasing{};

};