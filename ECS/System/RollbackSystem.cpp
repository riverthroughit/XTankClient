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
		//���з��������� ���շ�������������µ�Ԥ��
		rollbackComp.predictCmd = socketComp.curPlayersCmd;

	}

	auto& frameComp = mWorld->GetSingletonComponent<FrameComponent>();
	rollbackComp.predictCmd.frameId = frameComp.frameId;
	
	auto& inputComp = mWorld->GetSingletonComponent<InputComponent>();
	//��ǰ������ҵ�����
	rollbackComp.predictCmd.commandArray[socketComp.localPlayerId] = inputComp.curBtn;

	//����ǰԤ������������
	rollbackComp.predCmdDeq.push_back(rollbackComp.predictCmd);
}

void RollbackSystem::JudgeDuplicateWorld()
{
	auto& rollbackComp = mWorld->GetSingletonComponent<RollbackComponent>();
	rollbackComp.NeedResetDlctWorld = false;

	auto& socketComp = mWorld->GetSingletonComponent<SocketComponent>();

	//����ǰ�з��������� ���ж�Ԥ���Ƿ���ȷ
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
//	//�ñ��������Ѳ�����ʹ��
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
//	//ֱ�Ӹ���ǰһ֡�����
//	PlayersCommand predictedCmd = cmd;
//	++predictedCmd.frameId;
//	
//	//������������map�в���
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
//	//������ȷ������״̬
//	XTankWorld aHeadWorld = *rollbackComp.latestConfirmedWorld;
//	
//	//����ȷ��������Ԥ����һ֡����
//	rollbackComp.prePredictedCmd = CreatePredictedCmdFrom(rollbackComp.curPreciseCmd);
//	auto predictedCmd = rollbackComp.prePredictedCmd;
//
//	//����Ԥ�� ��������Ϸ�߼�֡
//	for (int i = rollbackComp.latestConfirmedFrameId + 1; i < frameComp.frameId; ++i) {
//		
//		aHeadWorld.SystemTickInRollback(predictedCmd);
//		predictedCmd = CreatePredictedCmdFrom(predictedCmd);
//
//	}
//
//	rollbackComp.curPredictedCmd = predictedCmd;
//
//	//������ȷ������״̬
//	XTankWorld ConfirmedWorld = *rollbackComp.latestConfirmedWorld;
//	//��Ԥ��״̬���赱ǰ��Ϸ����
//	static_cast<XTankWorld&>(*mWorld) = std::move(aHeadWorld);
//
//	auto& rollbackComp = mWorld->GetSingletonComponent<RollbackComponent>();
//	rollbackComp.latestConfirmedWorld = std::make_unique<XTankWorld>(std::move(ConfirmedWorld));
//}


 