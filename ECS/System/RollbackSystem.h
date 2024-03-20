#pragma once

#include "ECS/System.h"
#include "TypeConfig.h"
#include <vector>
#include <array>

class XTankWorld;

class RollbackSystem:public System<RollbackSystem> {

public:

	virtual void Tick(float dt)override;

	void Init()override;

	//获取复制的世界
	XTankWorld* GetDuplicateWorld();

	//更新预测世界
	void TickPredictWorld(float dt);

private:

	//更新服务器返回的准确命令
	void UpdatePreciseCmd();

	//更新预测的命令
	void UpdatePredictCmd();

	//根据命令预测下一命令
	std::array<BUTTON::Type, PLAYER_NUM> GetPredictCmdFrom(const std::array<BUTTON::Type, PLAYER_NUM>& preCmd);

	//判断预测是否成功 若不成功 则用当前权威命令重新预测至当前状态
	void RollbackPredictCmd();

	//预测下一帧指令
	void PredictNextCmd();

	//获取预测世界需执行的命令
	std::vector<PlayersCommand> GetPredictCmds();
};