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

	////��ǰ�з��������� ����Ԥ������
	//assert(HasCurCmd(socketComp) && !rollbackComp.predCmdDeq.empty());
	if (!rollbackComp.hasPreciseCmd || rollbackComp.predCmdDeq.empty()) {
		return;
	}

	PlayersCommand predictCmd = rollbackComp.predCmdDeq.front();

	//Ȩ��֡�ѵ� ��Ԥ��֡���Գ���
	rollbackComp.predCmdDeq.pop_front();

	//����׼ȷ ������Ԥ���������������
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

	//�������Ԥ������ ��Ԥ�� ��ǰ֡�Ѿ�Ԥ��� Ҳ�����ظ�Ԥ��
	if (!IsReachPredictLimit(rollbackComp) && !HasPredictCurFrame()) {

		//ע�� �ø��������frame���
		//auto& frameComp = rollbackComp.duplicateWorld->GetSingletonComponent<FrameComponent>();
		auto& frameComp = mWorld->GetSingletonComponent<FrameComponent>();
		
		//Ԥ������һ֡������
		PlayersCommand predCmd{};
		//predCmd.frameId = frameComp.serverFrameId + 1;
		predCmd.frameId = frameComp.clientTick.GetFrameId();
		rollbackComp.curPredictFrameId = predCmd.frameId;

		predCmd.commandArray = GetPredictCmdFrom(rollbackComp.preciseCmd.commandArray);
		auto& inputComp = mWorld->GetSingletonComponent<InputComponent>();
		//��ǰ������ҵ�����
		auto& socketComp = mWorld->GetSingletonComponent<SocketComponent>();
		predCmd.commandArray[socketComp.localPlayerId] = inputComp.curBtn;
		//����ǰԤ������������
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
	//��Ҫ�ع�
	//λ����������⴦�� �Ա��ֵƽ��

	auto& rollbackComp = mWorld->GetSingletonComponent<RollbackComponent>();

	if (!IsReachPredictLimit(rollbackComp)) {
		auto dupRollSystem = rollbackComp.duplicateWorld->GetSystem<RollbackSystem>();

		std::unordered_map<Entity, Vec2Fixed> prePoses;
		//��¼��ǰλ��
		for (Entity entity : dupRollSystem->mEntities) {
			const auto& posComp = rollbackComp.duplicateWorld->GetComponent<PosComponent>(entity);
			prePoses[entity] = posComp.pos;
		}

		//��Ȩ��״̬����
		*rollbackComp.duplicateWorld = XTankWorld(static_cast<XTankWorld&>(*mWorld));

		std::vector<PlayersCommand> cmds = GetPredictCmdsByNeedRollback(true);

		rollbackComp.duplicateWorld->SystemTickInDuplicate(dt, cmds);

		dupRollSystem = rollbackComp.duplicateWorld->GetSystem<RollbackSystem>();

		//�޸�Ԥ�������λ���������һ֡λ��
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

	//��Ԥ��ʧ�� ��������ִ����������
	if (needRollBack) {
		return std::vector<PlayersCommand>(rollbackComp.predCmdDeq.begin(), rollbackComp.predCmdDeq.end());
	}

	//����ֱ��ִ�е�ǰԤ�������
	return { rollbackComp.predCmdDeq.back() };
}


 