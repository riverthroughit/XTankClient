#include "RollbackSystem.h"
#include "ECS/XTankWorld.h"
#include "ECS/Component/RollbackComponent.h"
#include "ECS/Component/SocketComponent.h"
#include "ECS/Component/FrameComponent.h"
#include "ECS/Component/InputComponent.h"
#include <memory>

void RollbackSystem::Tick(float dt)
{

	UpdatePreciseCmd();
	UpdatePredictCmd();
	JudgeDuplicateWorld();
}

void RollbackSystem::Init()
{
	auto& rollbackComp = mWorld->GetSingletonComponent<RollbackComponent>();
	rollbackComp.duplicateWorld = new XTankWorld(static_cast<XTankWorld&>(*mWorld));
}

XTankWorld* RollbackSystem::DuplicateWorld()
{
	auto& rollbackComp = mWorld->GetSingletonComponent<RollbackComponent>();
	
	if (rollbackComp.NeedResetDlctWorld) {
		
		delete rollbackComp.duplicateWorld;
		rollbackComp.duplicateWorld = new XTankWorld(static_cast<XTankWorld&>(*mWorld));
	}

	return rollbackComp.duplicateWorld;
}

PlayersCommand RollbackSystem::GetPredictedCmd()
{
	return mWorld->GetSingletonComponent<RollbackComponent>().predictCmd;
}

void RollbackSystem::UpdatePreciseCmd()
{
	auto& socketComp = mWorld->GetSingletonComponent<SocketComponent>();
	auto& rollbackComp = mWorld->GetSingletonComponent<RollbackComponent>();

	rollbackComp.preciseCmd = {};

	if (socketComp.hasNewCmdMsg) {

		rollbackComp.preciseCmd = socketComp.curPlayersCmd;
	}
}

void RollbackSystem::UpdatePredictCmd()
{

	auto& rollbackComp = mWorld->GetSingletonComponent<RollbackComponent>();
	auto& socketComp = mWorld->GetSingletonComponent<SocketComponent>();

	if (socketComp.hasNewCmdMsg) {
		//若有服务器命令 则按照服务器命令进行新的预测
		rollbackComp.predictCmd = socketComp.curPlayersCmd;

	}

	auto& frameComp = mWorld->GetSingletonComponent<FrameComponent>();
	rollbackComp.predictCmd.frameId = frameComp.frameId;
	
	auto& inputComp = mWorld->GetSingletonComponent<InputComponent>();
	//当前本地玩家的命令
	rollbackComp.predictCmd.commandArray[socketComp.localPlayerId] = inputComp.curBtn;

	//将当前预测命令放入队列
	rollbackComp.predCmdDeq.push_back(rollbackComp.predictCmd);
}

void RollbackSystem::JudgeDuplicateWorld()
{
	auto& rollbackComp = mWorld->GetSingletonComponent<RollbackComponent>();
	rollbackComp.NeedResetDlctWorld = false;

	auto& socketComp = mWorld->GetSingletonComponent<SocketComponent>();

	//若当前有服务器命令 需判断预测是否正确
	if (socketComp.hasNewCmdMsg) {
		assert(!rollbackComp.predCmdDeq.empty());
		auto predCmd = rollbackComp.predCmdDeq.front();
		rollbackComp.predCmdDeq.pop_front();

		if (predCmd != rollbackComp.preciseCmd) {
			rollbackComp.NeedResetDlctWorld = true;
		}
	}
}


//void RollbackSystem::UpdateConfirmedWorld()
//{
//	auto& rollbackComp = mWorld->GetSingletonComponent<RollbackComponent>();
//
//	rollbackComp.latestConfirmedWorld->SystemTickInRollback(rollbackComp.curPreciseCmd);
//
//	//该本地命令已不会再使用
//	int count = rollbackComp.localPlayerCmdMap.erase(rollbackComp.latestConfirmedFrameId);
//	assert(count && "local command is not found");
//
//	++rollbackComp.latestConfirmedFrameId;
//}
//
//PlayersCommand RollbackSystem::CreatePredictedCmdFrom(const PlayersCommand& cmd)
//{
//
//	auto& socketComp = mWorld->GetSingletonComponent<SocketComponent>();
//	auto& rollbackComp = mWorld->GetSingletonComponent<RollbackComponent>();
//
//	//直接复制前一帧命令即可
//	PlayersCommand predictedCmd = cmd;
//	++predictedCmd.frameId;
//	
//	//本地玩家命令从map中查找
//	auto ite = rollbackComp.localPlayerCmdMap.find(predictedCmd.frameId);
//	assert(ite != rollbackComp.localPlayerCmdMap.end() && "local command is not found");
//
//	predictedCmd.commandArray[socketComp.localPlayerId] = ite->second;
//
//	return predictedCmd;
//}


//void RollbackSystem::RunAhead()
//{
//	auto& rollbackComp = mWorld->GetSingletonComponent<RollbackComponent>();
//	auto& frameComp = mWorld->GetSingletonComponent<FrameComponent>();
//
//	//拷贝已确定世界状态
//	XTankWorld aHeadWorld = *rollbackComp.latestConfirmedWorld;
//	
//	//用已确定的命令预测下一帧命令
//	rollbackComp.prePredictedCmd = CreatePredictedCmdFrom(rollbackComp.curPreciseCmd);
//	auto predictedCmd = rollbackComp.prePredictedCmd;
//
//	//不断预测 至本地游戏逻辑帧
//	for (int i = rollbackComp.latestConfirmedFrameId + 1; i < frameComp.frameId; ++i) {
//		
//		aHeadWorld.SystemTickInRollback(predictedCmd);
//		predictedCmd = CreatePredictedCmdFrom(predictedCmd);
//
//	}
//
//	rollbackComp.curPredictedCmd = predictedCmd;
//
//	//拷贝已确定世界状态
//	XTankWorld ConfirmedWorld = *rollbackComp.latestConfirmedWorld;
//	//将预测状态赋予当前游戏世界
//	static_cast<XTankWorld&>(*mWorld) = std::move(aHeadWorld);
//
//	auto& rollbackComp = mWorld->GetSingletonComponent<RollbackComponent>();
//	rollbackComp.latestConfirmedWorld = std::make_unique<XTankWorld>(std::move(ConfirmedWorld));
//}


 