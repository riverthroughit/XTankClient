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

	UpdatePreciseCmd();
	UpdatePredictCmd();
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

	if (HasNewCmdMsg(socketComp)) {

		rollbackComp.preciseCmd = socketComp.curPlayersCmd;
	}
}

void RollbackSystem::UpdatePredictCmd()
{


	//�ж�֮ǰ�������Ƿ�Ԥ��ɹ�
	RollbackPredictCmd();

	PredictNextCmd();


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
	rollbackComp.NeedResetDlctWorld = false;

	if (rollbackComp.predCmdDeq.empty()) {
		return;
	}

	auto& socketComp = mWorld->GetSingletonComponent<SocketComponent>();

	//����ǰ�з��������� ���ж�Ԥ���Ƿ���ȷ
	if (HasNewCmdMsg(socketComp)) {
		assert(!rollbackComp.predCmdDeq.empty());

		PlayersCommand predictCmd = rollbackComp.predCmdDeq.front();
		//Ȩ��֡�ѵ� ��Ԥ��֡���Գ���
		rollbackComp.predCmdDeq.pop_front();

		//����׼ȷ ������Ԥ���������������
		if (predictCmd != rollbackComp.preciseCmd) {
			
			rollbackComp.NeedResetDlctWorld = true;
			PlayersCommand preCmd = rollbackComp.preciseCmd;
			
			for (PlayersCommand& cmd : rollbackComp.predCmdDeq) {
				
				BUTTON::Type localBtn = cmd.commandArray[socketComp.localPlayerId];
				cmd.commandArray = GetPredictCmdFrom(preCmd.commandArray);
				cmd.commandArray[socketComp.localPlayerId] = localBtn;
				preCmd = cmd;
			}
		}
	}
}

void RollbackSystem::PredictNextCmd()
{

	auto& rollbackComp = mWorld->GetSingletonComponent<RollbackComponent>();

	//�������Ԥ������ ��Ԥ��
	if (!IsReachPredictLimit(rollbackComp)) {

		//ע�� �ø��������frame���
		//auto& frameComp = rollbackComp.duplicateWorld->GetSingletonComponent<FrameComponent>();
		auto& frameComp = mWorld->GetSingletonComponent<FrameComponent>();
		
		//Ԥ������һ֡������
		PlayersCommand predCmd{};
		//predCmd.frameId = frameComp.serverFrameId + 1;
		predCmd.frameId = frameComp.clientTick.GetFrameId();
		predCmd.commandArray = GetPredictCmdFrom(rollbackComp.preciseCmd.commandArray);
		auto& inputComp = mWorld->GetSingletonComponent<InputComponent>();
		//��ǰ������ҵ�����
		auto& socketComp = mWorld->GetSingletonComponent<SocketComponent>();
		predCmd.commandArray[socketComp.localPlayerId] = inputComp.curBtn;
		//����ǰԤ������������
		rollbackComp.predCmdDeq.push_back(predCmd);
	}
}

void RollbackSystem::TickPredictWorld(float dt)
{
	auto& rollbackComp = mWorld->GetSingletonComponent<RollbackComponent>();
	
	//�ﵽ���� ֱ�ӷ���
	if (IsReachPredictLimit(rollbackComp)) {
		return;
	}

	std::vector<PlayersCommand> cmds = GetPredictCmds();
	
	if (!rollbackComp.NeedResetDlctWorld) {
		//���ûع������
		rollbackComp.duplicateWorld->SystemTickInDuplicate(dt, cmds);
	}
	else {
		//��Ҫ�ع�
		//λ����������⴦�� �Ա��ֵƽ��
		
		auto dupRollSystem = rollbackComp.duplicateWorld->GetSystem<RollbackSystem>();

		std::unordered_map<Entity, Vec2Fixed> prePoses;
		//��¼��ǰλ��
		for (Entity entity : dupRollSystem->mEntities) {
			const auto& posComp = rollbackComp.duplicateWorld->GetComponent<PosComponent>(entity);
			prePoses[entity] = posComp.pos;
		}
		
		//��Ȩ��״̬����
		*rollbackComp.duplicateWorld = XTankWorld(static_cast<XTankWorld&>(*mWorld));
		rollbackComp.duplicateWorld->SystemTickInDuplicate(dt, cmds);
		
		dupRollSystem = rollbackComp.duplicateWorld->GetSystem<RollbackSystem>();

		//�޸�Ԥ�������λ���������һ֡λ��
		for (Entity entity : dupRollSystem->mEntities) {
			auto& posComp = rollbackComp.duplicateWorld->GetComponent<PosComponent>(entity);
			posComp.prePos = prePoses[entity];
		}
	}
}

std::vector<PlayersCommand> RollbackSystem::GetPredictCmds()
{
	auto& rollbackComp = mWorld->GetSingletonComponent<RollbackComponent>();

	//��Ԥ��ʧ�� ��������ִ����������
	if (rollbackComp.NeedResetDlctWorld) {
		return std::vector<PlayersCommand>(rollbackComp.predCmdDeq.begin(), rollbackComp.predCmdDeq.end());
	}

	//����ֱ��ִ�е�ǰԤ�������
	return { rollbackComp.predCmdDeq.back() };
}


 