#pragma once
#include <memory>
#include <map>
#include <deque>
#include "TypeConfig.h"

class XTankWorld;

struct RollbackComponent {

	//预测的世界
	XTankWorld* duplicateWorld{};

	//是否需要重置预测的世界（当发现预测错误时）
	bool NeedResetDlctWorld{};

	//预测的命令队列
	std::deque<PlayersCommand> predCmdDeq{};

	//当前到来的服务器命令
	PlayersCommand preciseCmd{};

	//当前预测的命令
	PlayersCommand predictCmd{};

};