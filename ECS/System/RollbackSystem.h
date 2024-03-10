#pragma once

#include "ECS/System.h"
#include "TypeConfig.h"

class RollbackSystem:public System {

public:

	virtual void Tick(float dt)override;

private:

	//更新当前得到的准确命令
	void UpdatePreciseCmd();

	//更新本地用户命令
	void UpdateLocalCmd();

	//操作不一致后回滚至正确状态
	void Rollback();

	//生成预测命令
	PlayersCommand CreatePredictedCmdFrom(const PlayersCommand& cmd);

	//用之前的网络输入和当前的本地输入预测
	void RunAhead();

};