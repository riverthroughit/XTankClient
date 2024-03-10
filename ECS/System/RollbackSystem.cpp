#include "RollbackSystem.h"
#include "ECS/Component/RollbackComponent.h"
#include "ECS/Component/SocketComponent.h"
#include "ECS/Component/FrameComponent.h"
#include "ECS/Component/InputComponent.h"
#include <memory>

void RollbackSystem::Tick(float dt)
{

	auto& rollbackComp = mWorld->GetSingletonComponent<RollbackComponent>();

	UpdatePreciseCmd();
	UpdateLocalCmd();

	if (rollbackComp.hasPreciseCmd 
		&& rollbackComp.curPreciseCmd != rollbackComp.prePredictedCmd) {
		
		//与预测的命令不相等 进行回滚
		Rollback();
	
	}
	
	RunAhead();

}

void RollbackSystem::UpdatePreciseCmd()
{
	auto& socketComp = mWorld->GetSingletonComponent<SocketComponent>();
	auto& rollbackComp = mWorld->GetSingletonComponent<RollbackComponent>();

	rollbackComp.hasPreciseCmd = false;

	if (socketComp.hasNewCmdMsg) {
		rollbackComp.curPreciseCmd = socketComp.curPlayersCmd;
		rollbackComp.hasPreciseCmd = true;
	}
}

void RollbackSystem::UpdateLocalCmd()
{
	auto& frameComp = mWorld->GetSingletonComponent<FrameComponent>();
	auto& rollbackComp = mWorld->GetSingletonComponent<RollbackComponent>();
	auto& inputComp = mWorld->GetSingletonComponent<InputComponent>();

	rollbackComp.localPlayerCmdMap.insert({ frameComp.frameId,inputComp.curBtn });
}

void RollbackSystem::Rollback()
{
	auto& rollbackComp = mWorld->GetSingletonComponent<RollbackComponent>();

	rollbackComp.latestConfirmedWorld->SystemTickInRollback(rollbackComp.curPreciseCmd);

	//该本地命令已不会再使用
	int count = rollbackComp.localPlayerCmdMap.erase(rollbackComp.latestConfirmedFrameId);
	assert(count && "local command is not found");

	++rollbackComp.latestConfirmedFrameId;
}

PlayersCommand RollbackSystem::CreatePredictedCmdFrom(const PlayersCommand& cmd)
{

	auto& socketComp = mWorld->GetSingletonComponent<SocketComponent>();
	auto& rollbackComp = mWorld->GetSingletonComponent<RollbackComponent>();

	//直接复制前一帧命令即可
	PlayersCommand predictedCmd = cmd;
	++predictedCmd.frameId;
	
	//本地玩家命令从map中查找
	auto ite = rollbackComp.localPlayerCmdMap.find(predictedCmd.frameId);
	assert(ite != rollbackComp.localPlayerCmdMap.end() && "local command is not found");

	predictedCmd.commandArray[socketComp.localPlayerId] = ite->second;

	return predictedCmd;
}

void RollbackSystem::RunAhead()
{
	auto& rollbackComp = mWorld->GetSingletonComponent<RollbackComponent>();
	auto& frameComp = mWorld->GetSingletonComponent<FrameComponent>();

	//拷贝已确定世界状态
	std::shared_ptr<XTankWorld> aHeadWorld = std::make_shared<XTankWorld>(*rollbackComp.latestConfirmedWorld);
	
	//用已确定的命令预测下一帧命令
	rollbackComp.prePredictedCmd = CreatePredictedCmdFrom(rollbackComp.curPreciseCmd);
	auto predictedCmd = rollbackComp.prePredictedCmd;

	//不断预测 至本地游戏逻辑帧
	for (int i = rollbackComp.latestConfirmedFrameId; i < frameComp.frameId; ++i) {
		aHeadWorld->SystemTickInRollback(predictedCmd);
		predictedCmd = CreatePredictedCmdFrom(predictedCmd);
	}

	rollbackComp.curPredictedCmd = CreatePredictedCmdFrom(predictedCmd);

	//将预测状态赋予当前游戏世界
	XTankWorld& tankWorld = static_cast<XTankWorld&>(*mWorld);
	tankWorld = std::move(*aHeadWorld);
}


 