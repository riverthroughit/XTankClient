#include "RollbackSystem.h"
#include "ECS/XTankWorld.h"
#include "ECS/Component/RollbackComponent.h"
#include "ECS/Component/SocketComponent.h"
#include "ECS/Component/InputComponent.h"
#include "ECS/Component/PosComponent.h"
#include "ECS/Component/FrameComponent.h"
#include "ECS/Component/RollbackPosComponent.h"
#include "ECS/ECSUtil.h"
#include <memory>


void RollbackSystem::Tick(float dt)
{

	//UpdatePreciseCmd();
	//UpdatePredictCmd();
}

void RollbackSystem::Init()
{
	auto& rollbackComp = mWorld->GetSingletonComponent<RollbackComponent>();
	rollbackComp.duplicateWorld = new XTankWorld(static_cast<XTankWorld&>(*mWorld));
	rollbackComp.maxPredictTimes = MAX_PREDICT_FRAME;
	rollbackComp.preciseCmd = { -1,std::array<BUTTON::Type, PLAYER_NUM>{BUTTON::NONE} };
}

XTankWorld* RollbackSystem::GetDuplicateWorld()
{
	auto& rollbackComp = mWorld->GetSingletonComponent<RollbackComponent>();

	return rollbackComp.duplicateWorld;
}

void RollbackSystem::UpdatePreciseCmd()
{
	auto& socketComp = mWorld->GetSingletonComponent<SocketComponent>();
	auto& rollbackComp = mWorld->GetSingletonComponent<RollbackComponent>();

	rollbackComp.hasPreciseCmd = false;

	if (HasCurCmd(socketComp)) {

		rollbackComp.preciseCmd = GetSocketCurCmd(socketComp);
		rollbackComp.hasPreciseCmd = true;

		RollbackPredictCmd();
	}
}


std::array<BUTTON::Type, PLAYER_NUM> RollbackSystem::GetPredictCmdFrom(const std::array<BUTTON::Type, PLAYER_NUM>& preCmd)
{
	std::array<BUTTON::Type, PLAYER_NUM> predictCmd{};

	for (int i = 0; i < PLAYER_NUM; ++i) {
		
		if (preCmd[i] != BUTTON::CUT_IN && preCmd[i] != BUTTON::FIRE) {
			predictCmd[i] = preCmd[i];
		}
	}

	return predictCmd;
}

void RollbackSystem::RollbackPredictCmd()
{
	auto& rollbackComp = mWorld->GetSingletonComponent<RollbackComponent>();
	auto& socketComp = mWorld->GetSingletonComponent<SocketComponent>();
	
	rollbackComp.NeedRollback = false;

	////当前有服务器命令 且有预测命令
	//assert(HasCurCmd(socketComp) && !rollbackComp.predCmdDeq.empty());
	if (!rollbackComp.hasPreciseCmd || rollbackComp.predCmdDeq.empty()) {
		return;
	}

	PlayersCommand predictCmd = rollbackComp.predCmdDeq.front();

	//权威帧已到 该预测帧可以出队
	rollbackComp.predCmdDeq.pop_front();

	//若不准确 则重新预测往后的所有命令
	if (predictCmd != rollbackComp.preciseCmd) {

		rollbackComp.NeedRollback = true;
		PlayersCommand preCmd = rollbackComp.preciseCmd;

		for (PlayersCommand& cmd : rollbackComp.predCmdDeq) {

			BUTTON::Type localBtn = cmd.commandArray[socketComp.localPlayerId];
			cmd.commandArray = GetPredictCmdFrom(preCmd.commandArray);
			cmd.commandArray[socketComp.localPlayerId] = localBtn;
			preCmd = cmd;
		}
	}
}

void RollbackSystem::PredictNextCmd()
{

	auto& rollbackComp = mWorld->GetSingletonComponent<RollbackComponent>();

	//到达最大预测上限 则不预测 当前帧已经预测过 也不再重复预测
	if (!IsReachPredictLimit(rollbackComp) && !HasPredictCurFrame()) {

		//注意 用副本世界的frame组件
		//auto& frameComp = rollbackComp.duplicateWorld->GetSingletonComponent<FrameComponent>();
		auto& frameComp = mWorld->GetSingletonComponent<FrameComponent>();
		
		//预测最新一帧的命令
		PlayersCommand predCmd{};
		//predCmd.frameId = frameComp.serverFrameId + 1;
		predCmd.frameId = frameComp.clientTick.GetFrameId();
		rollbackComp.curPredictFrameId = predCmd.frameId;

		predCmd.commandArray = GetPredictCmdFrom(rollbackComp.preciseCmd.commandArray);
		auto& inputComp = mWorld->GetSingletonComponent<InputComponent>();
		//当前本地玩家的命令
		auto& socketComp = mWorld->GetSingletonComponent<SocketComponent>();
		predCmd.commandArray[socketComp.localPlayerId] = inputComp.curBtn;
		//将当前预测命令放入队列
		rollbackComp.predCmdDeq.push_back(predCmd);
	}
}

bool RollbackSystem::HasPredictCurFrame()
{
	auto& rollbackComp = mWorld->GetSingletonComponent<RollbackComponent>();
	auto& frameComp = mWorld->GetSingletonComponent<FrameComponent>();

	return rollbackComp.curPredictFrameId == frameComp.clientTick.GetFrameId();
}

void RollbackSystem::RunAheadPredictWorld(float dt)
{
	auto& rollbackComp = mWorld->GetSingletonComponent<RollbackComponent>();

	if (!IsReachPredictLimit(rollbackComp)) {
		
		std::vector<PlayersCommand> cmds = GetPredictCmdsByNeedRollback(false);
		rollbackComp.duplicateWorld->SystemTickInDuplicate(dt, cmds);
	}
}

void RollbackSystem::RollbackPredictWorld(float dt)
{
	//需要回滚
	//位置组件需特殊处理 以便插值平滑

	auto& rollbackComp = mWorld->GetSingletonComponent<RollbackComponent>();

	if (!IsReachPredictLimit(rollbackComp)) {
		auto dupRollSystem = rollbackComp.duplicateWorld->GetSystem<RollbackSystem>();

		std::unordered_map<Entity, Vec2Fixed> prePoses;
		//记录当前位置
		for (Entity entity : dupRollSystem->mEntities) {
			const auto& posComp = rollbackComp.duplicateWorld->GetComponent<PosComponent>(entity);
			prePoses[entity] = posComp.pos;
		}

		//用权威状态覆盖
		*rollbackComp.duplicateWorld = XTankWorld(static_cast<XTankWorld&>(*mWorld));

		std::vector<PlayersCommand> cmds = GetPredictCmdsByNeedRollback(true);

		rollbackComp.duplicateWorld->SystemTickInDuplicate(dt, cmds);

		dupRollSystem = rollbackComp.duplicateWorld->GetSystem<RollbackSystem>();

		//修改预测世界的位置组件的上一帧位置
		for (Entity entity : dupRollSystem->mEntities) {
			auto& posComp = rollbackComp.duplicateWorld->GetComponent<PosComponent>(entity);
			posComp.prePos = prePoses[entity];
		}
	}
}

void RollbackSystem::TryRollbackPredictWorld(float dt)
{
	auto& rollbackComp = mWorld->GetSingletonComponent<RollbackComponent>();

	if (rollbackComp.NeedRollback) {
		RollbackPredictWorld(dt);
	}
}

std::vector<PlayersCommand> RollbackSystem::GetPredictCmdsByNeedRollback(bool needRollBack)
{
	auto& rollbackComp = mWorld->GetSingletonComponent<RollbackComponent>();

	//若预测失败 则需重新执行所有命令
	if (needRollBack) {
		return std::vector<PlayersCommand>(rollbackComp.predCmdDeq.begin(), rollbackComp.predCmdDeq.end());
	}

	//否则直接执行当前预测命令即可
	return { rollbackComp.predCmdDeq.back() };
}


 