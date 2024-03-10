#pragma once

#include "ECS/XTankWorld.h"
#include <memory>
#include <utility>
#include <map>
#include <array>
#include "TypeConfig.h"

struct RollbackComponent {

	//最新确认的准确的游戏世界
	std::shared_ptr<XTankWorld> latestConfirmedWorld{};
	//最新确认的世界对应帧数
	unsigned int latestConfirmedFrameId{};

	//对应frameId为lastConfirmedFrameId时 预测的命令
	PlayersCommand prePredictedCmd{};

	//当前是否有服务器命令到来
	bool hasPreciseCmd{};

	//当前到来的服务器命令
	PlayersCommand curPreciseCmd{};

	//当前帧预测的命令
	PlayersCommand curPredictedCmd{};

	//本地用户的命令 {frameId,cmd}
	std::map<int,BUTTON::Type> localPlayerCmdMap;

};