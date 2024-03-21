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

	//没有预测失败 继续更新预测世界
	void RunAheadPredictWorld(float dt);

	//预测失败 需回滚预测世界
	void RollbackPredictWorld(float dt);

	//尝试回滚预测世界 若预测成功 不回滚 若失败 回滚
	void TryRollbackPredictWorld(float dt);

	//更新服务器返回的准确命令
	void UpdatePreciseCmd();

	//判断预测是否成功 若不成功 则用当前权威命令重新预测至当前状态
	void RollbackPredictCmd();

	//预测下一帧指令
	void PredictNextCmd();

private:

	//根据命令预测下一命令
	std::array<BUTTON::Type, PLAYER_NUM> GetPredictCmdFrom(const std::array<BUTTON::Type, PLAYER_NUM>& preCmd);

	//当前帧是否已经预测过
	bool HasPredictCurFrame();

	//根据是否需要回滚获取预测世界需执行的命令
	std::vector<PlayersCommand> GetPredictCmdsByNeedRollback(bool needRollBack);
};