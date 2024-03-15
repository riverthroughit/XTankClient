#include "RollbackSystem.h"
#include "ECS/XTankWorld.h"
#include "ECS/Component/RollbackComponent.h"
#include "ECS/Component/SocketComponent.h"
#include "ECS/Component/InputComponent.h"
#include "ECS/Component/PosComponent.h"
#include "ECS/Component/FrameComponent.h"
#include <memory>


void RollbackSystem::Tick(float dt)
{

	UpdatePreciseCmd();
	UpdatePredictCmd();
}

void RollbackSystem::Init()
{
	auto& rollbackComp = mWorld->GetSingletonComponent<RollbackComponent>();
	rollbackComp.duplicateWorld = new XTankWorld(static_cast<XTankWorld&>(*mWorld));
	rollbackComp.maxPredictTimes = 5;
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

	if (socketComp.hasNewCmdMsg) {

		rollbackComp.preciseCmd = socketComp.curPlayersCmd;
	}
}

void RollbackSystem::UpdatePredictCmd()
{

	auto& rollbackComp = mWorld->GetSingletonComponent<RollbackComponent>();
	auto& socketComp = mWorld->GetSingletonComponent<SocketComponent>();

	//判断之前的命令是否预测成功
	RollbackPredictCmd();

	//预测最新一帧的命令
	PlayersCommand predCmd{};
	auto& frameComp = mWorld->GetSingletonComponent<FrameComponent>();
	

	//到达最大预测上限 则预测空指令
	if (rollbackComp.predCmdDeq.size() >= rollbackComp.maxPredictTimes) {
		
		predCmd.frameId = frameComp.frameId;
	}
	else {

		predCmd = rollbackComp.preciseCmd;
		predCmd.frameId = frameComp.frameId;

		auto& inputComp = mWorld->GetSingletonComponent<InputComponent>();
		//当前本地玩家的命令
		predCmd.commandArray[socketComp.localPlayerId] = inputComp.curBtn;

	}

	//将当前预测命令放入队列
	rollbackComp.predCmdDeq.push_back(predCmd);

}

void RollbackSystem::RollbackPredictCmd()
{
	auto& rollbackComp = mWorld->GetSingletonComponent<RollbackComponent>();
	rollbackComp.NeedResetDlctWorld = false;

	auto& socketComp = mWorld->GetSingletonComponent<SocketComponent>();

	//若当前有服务器命令 需判断预测是否正确
	if (socketComp.hasNewCmdMsg) {
		assert(!rollbackComp.predCmdDeq.empty());

		PlayersCommand predictCmd = rollbackComp.predCmdDeq.front();
		//权威帧已到 该预测帧可以出队
		rollbackComp.predCmdDeq.pop_front();

		//若不准确 则重新预测往后的所有命令
		if (predictCmd != rollbackComp.preciseCmd) {
			
			rollbackComp.NeedResetDlctWorld = true;
			PlayersCommand preCmd = rollbackComp.preciseCmd;
			
			for (PlayersCommand& cmd : rollbackComp.predCmdDeq) {
				
				BUTTON::Type localBtn = cmd.commandArray[socketComp.localPlayerId];
				int framId = cmd.frameId;
				cmd = preCmd;
				cmd.frameId = framId;
				cmd.commandArray[socketComp.localPlayerId] = localBtn;
				preCmd = cmd;
			}
		}
	}
}

void RollbackSystem::TickPredictWorld(float dt)
{
	auto& rollbackComp = mWorld->GetSingletonComponent<RollbackComponent>();
	
	std::vector<PlayersCommand> cmds = GetPredictCmds();
	
	if (!rollbackComp.NeedResetDlctWorld) {
		//不用回滚的情况
		rollbackComp.duplicateWorld->SystemTickInDuplicate(dt, cmds);
	}
	else {
		//需要回滚
		//位置组件需特殊处理 以便插值平滑 {entity,{prePos,preDirec}}
		std::unordered_map<Entity, std::pair<Vec2Fixed, Vec2Fixed>> prePosMap;
		
		auto dupRollSystem = rollbackComp.duplicateWorld->GetSystem<RollbackSystem>();

		//记录当前位置
		for (Entity entity : dupRollSystem->mEntities) {
			const auto& posComp = rollbackComp.duplicateWorld->GetComponent<PosComponent>(entity);
			prePosMap.insert({ entity,{posComp.pos,posComp.direc} });
		}

		delete rollbackComp.duplicateWorld;
		rollbackComp.duplicateWorld = new XTankWorld(static_cast<XTankWorld&>(*mWorld));
		rollbackComp.duplicateWorld->SystemTickInDuplicate(dt, cmds);
		dupRollSystem = rollbackComp.duplicateWorld->GetSystem<RollbackSystem>();

		//修改预测世界的位置组件的上一帧位置
		for (Entity entity : dupRollSystem->mEntities) {
			
			auto ite = prePosMap.find(entity);
			if (ite != prePosMap.end()) {

				auto& [pos, direc] = ite->second;

				auto& posComp = rollbackComp.duplicateWorld->GetComponent<PosComponent>(entity);
				posComp.prePos = pos;
				posComp.preDirec = direc;
			}
		}
	}
}

std::vector<PlayersCommand> RollbackSystem::GetPredictCmds()
{
	auto& rollbackComp = mWorld->GetSingletonComponent<RollbackComponent>();

	//若预测失败 则需重新执行所有命令
	if (rollbackComp.NeedResetDlctWorld) {
		return std::vector<PlayersCommand>(rollbackComp.predCmdDeq.begin(), rollbackComp.predCmdDeq.end());
	}

	//否则直接执行当前预测命令即可
	return { rollbackComp.predCmdDeq.back() };
}


 