#pragma once

#include "ECS/System.h"
#include "TypeConfig.h"

class XTankWorld;

class RollbackSystem:public System<RollbackSystem> {

public:

	virtual void Tick(float dt)override;

	void Init()override;

	//复制世界并返回 需在真实世界更新后调用
	XTankWorld* DuplicateWorld();

	//返回预测的命令
	PlayersCommand GetPredictedCmd();

private:

	//更新服务器返回的准确命令
	void UpdatePreciseCmd();

	//更新预测的命令
	void UpdatePredictCmd();

	//更新预测的世界
	void JudgeDuplicateWorld();

};