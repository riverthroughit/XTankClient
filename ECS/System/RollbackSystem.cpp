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
		
		//��Ԥ��������� ���лع�
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

	//�ñ��������Ѳ�����ʹ��
	int count = rollbackComp.localPlayerCmdMap.erase(rollbackComp.latestConfirmedFrameId);
	assert(count && "local command is not found");

	++rollbackComp.latestConfirmedFrameId;
}

PlayersCommand RollbackSystem::CreatePredictedCmdFrom(const PlayersCommand& cmd)
{

	auto& socketComp = mWorld->GetSingletonComponent<SocketComponent>();
	auto& rollbackComp = mWorld->GetSingletonComponent<RollbackComponent>();

	//ֱ�Ӹ���ǰһ֡�����
	PlayersCommand predictedCmd = cmd;
	++predictedCmd.frameId;
	
	//������������map�в���
	auto ite = rollbackComp.localPlayerCmdMap.find(predictedCmd.frameId);
	assert(ite != rollbackComp.localPlayerCmdMap.end() && "local command is not found");

	predictedCmd.commandArray[socketComp.localPlayerId] = ite->second;

	return predictedCmd;
}

void RollbackSystem::RunAhead()
{
	auto& rollbackComp = mWorld->GetSingletonComponent<RollbackComponent>();
	auto& frameComp = mWorld->GetSingletonComponent<FrameComponent>();

	//������ȷ������״̬
	std::shared_ptr<XTankWorld> aHeadWorld = std::make_shared<XTankWorld>(*rollbackComp.latestConfirmedWorld);
	
	//����ȷ��������Ԥ����һ֡����
	rollbackComp.prePredictedCmd = CreatePredictedCmdFrom(rollbackComp.curPreciseCmd);
	auto predictedCmd = rollbackComp.prePredictedCmd;

	//����Ԥ�� ��������Ϸ�߼�֡
	for (int i = rollbackComp.latestConfirmedFrameId; i < frameComp.frameId; ++i) {
		aHeadWorld->SystemTickInRollback(predictedCmd);
		predictedCmd = CreatePredictedCmdFrom(predictedCmd);
	}

	rollbackComp.curPredictedCmd = CreatePredictedCmdFrom(predictedCmd);

	//��Ԥ��״̬���赱ǰ��Ϸ����
	XTankWorld& tankWorld = static_cast<XTankWorld&>(*mWorld);
	tankWorld = std::move(*aHeadWorld);
}


 