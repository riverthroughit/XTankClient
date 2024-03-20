#include "SocketSystem.h"
#include "ECS/Component/SocketComponent.h"
#include "Socket/MessageQueue.h"
#include "Socket/SocketClient.h"
#include "ECS/World.h"
#include "Protobuf/XTankMsg.pb.h"
#include <windows.h>
#include "ECS/Component/InputComponent.h"
#include "ECS/Component/FrameComponent.h"
#include "ECS/Component/RandComponent.h"
#include "ECS/Component/RollbackComponent.h"
#include "ECS/ECSUtil.h"

using google::protobuf::Message;

void SocketSystem::Init()
{
	auto& socketComp = mWorld->GetSingletonComponent<SocketComponent>();
	
	//�ȴ�ֱ��������������ʼ��֪ͨ
	MessageData msgData = MsgRecvQueue::Instance().GetAndPopTopMsg();
	while (msgData.type != XTankMsg::GAME_INIT_NTF) {
		msgData = MsgRecvQueue::Instance().GetAndPopTopMsg();
	}

	std::shared_ptr<XTankMsg::GameInitNtf> msgPtr = std::static_pointer_cast<XTankMsg::GameInitNtf>(msgData.msgPtr);
	
	//���������
	auto& randComp = mWorld->GetSingletonComponent<RandComponent>();
	randComp.seed = msgPtr->randseed();

	const auto& playerIds = msgPtr->playerids();
	socketComp.playerNum = playerIds.size();

	SetLocalPlayerIdByMsg(playerIds);

}

void SocketSystem::Tick(float dt)
{
	ReceiveCmd();
	SendCmd();

}

void SocketSystem::ReceiveCmd()
{
	auto& socketComp = mWorld->GetSingletonComponent<SocketComponent>();

	//���ܵ�ǰ�ѵ������������

	while (1) {
		MessageData msgData = MsgRecvQueue::Instance().GetAndPopTopMsg();
		if (msgData.type == XTankMsg::NONE) {
			break;
		}
		else if (msgData.type == XTankMsg::GAME_FORWARD_NTF) {
			PlayersCommand cmd = GetCmdFromMsgData(msgData);

			//��ֹ�յ��ظ����� (׷֡ʱ)
			assert(cmd.frameId > socketComp.curServerFrameId);
			socketComp.playersCmdsBuffer.push_back(cmd);
			//break;
		}
	}

	UpdateCurPlayersCmd();

}

void SocketSystem::SendCmd()
{

	auto& socketComp = mWorld->GetSingletonComponent<SocketComponent>();

	//׷֡ʱ �����ͱ�����Ҳ���     
	if (IsChasingEnd()) {

		//�Ƿ�Ϊ��;���뿪��׷֡
		if (!socketComp.isCutInChasing) {

			//auto& rollbackComp = mWorld->GetSingletonComponent<RollbackComponent>();
			////�ﵽԤ������ʱ������
			//if (!IsReachPredictLimit(rollbackComp)) {

				SendLocalPlayerCmd();
			//}
		}
		else {
			socketComp.isCutInChasing = false;
			//֪ͨ������׷֡���
			MsgSendQueue::Instance().SendPlayerChaseUpNtf();
		}
	}
}

bool SocketSystem::HasNewCmdMsg()
{
	auto& socketComp = mWorld->GetSingletonComponent<SocketComponent>();
	return socketComp.hasNewCmdMsg;
}

void SocketSystem::WaitStart()
{
	//��׼�����
	MsgSendQueue::Instance().SendGameReadyAck();

	MessageData msgData;
	
	while (1) {

		msgData = MsgRecvQueue::Instance().GetAndPopTopMsg();
		
		if (msgData.type == XTankMsg::GAME_START_NTF) {
			//����֪ͨ
			break;
		}
		else if (msgData.type == XTankMsg::PLAYER_CUT_IN_NTF) {
			//�����Ϊ��;���� ��ȡ�䷿����id�Լ�׷֡��Ϣ

			InitCutInData(msgData);

			break;

		}
	}
}

bool SocketSystem::NeedChasing()
{
	auto& socketComp = mWorld->GetSingletonComponent<SocketComponent>();

	return !socketComp.playersCmdsBuffer.empty();
}

bool SocketSystem::IsChasingEnd()
{
	auto& socketComp = mWorld->GetSingletonComponent<SocketComponent>();

	return socketComp.playersCmdsBuffer.empty();
}

float SocketSystem::GetTickTimeBasedOnChasing()
{
	auto& socketComp = mWorld->GetSingletonComponent<SocketComponent>();

	int bufferSize = socketComp.playersCmdsBuffer.size();
	float tick = LOCKSTEP_TICK / (bufferSize + 1);
	return MIN_CHASING_TICK > tick ? MIN_CHASING_TICK : tick;
}


PlayersCommand SocketSystem::GetCmdFromMsgData(const MessageData& msgData)
{
	auto msgPtr = std::static_pointer_cast<XTankMsg::GameForwardNtf>(msgData.msgPtr);

	PlayersCommand cmd;
	cmd.frameId = msgPtr->frameid();
	const auto& playersCmd = msgPtr->playercmds();
	for (int i = 0; i < playersCmd.size(); ++i) {

		cmd.commandArray[i] = static_cast<BUTTON::Type>(playersCmd.Get(i));
	}

	return cmd;
}

void SocketSystem::UpdateCurPlayersCmd()
{
	auto& socketComp = mWorld->GetSingletonComponent<SocketComponent>();

	if (socketComp.playersCmdsBuffer.empty()) {
		//��ǰ֡û���յ�������ת��ָ��
		socketComp.hasNewCmdMsg = false;
		socketComp.curPlayersCmd = {};
		return;
	}

	socketComp.hasNewCmdMsg = true;
	socketComp.curPlayersCmd = socketComp.playersCmdsBuffer.front();
	socketComp.curServerFrameId = socketComp.curPlayersCmd.frameId;
	socketComp.playersCmdsBuffer.pop_front();

	//�ж��Ƿ�����Ҽ���ָ�� ���� �����ӱ�����������
	for (auto cmdType : socketComp.curPlayersCmd.commandArray) {

		switch (cmdType)
		{
		case BUTTON::CUT_IN:
			++socketComp.playerNum;
			break;

		case BUTTON::EXIT:
			--socketComp.playerNum;
			break;

		default:
			break;
		}
	}
}

void SocketSystem::SendLocalPlayerCmd()
{


	auto& frameComp = mWorld->GetSingletonComponent<FrameComponent>();
	int clientFrameId = frameComp.clientTick.GetFrameId();

	auto& socketComp = mWorld->GetSingletonComponent<SocketComponent>();

	//����ÿֻ֡�ܷ���һ��
	if (socketComp.lastSendFrameId < clientFrameId) {
		
		socketComp.lastSendFrameId = clientFrameId;

		auto& inputComp = mWorld->GetSingletonComponent<InputComponent>();
		MsgSendQueue::Instance().SendPlayerInputNtf(clientFrameId, inputComp.curBtn);
	}

}

void SocketSystem::InitCutInData(const MessageData& msgData)
{

	auto msgPtr = std::static_pointer_cast<XTankMsg::PlayerCutInNtf>(msgData.msgPtr);
	auto& socketComp = mWorld->GetSingletonComponent<SocketComponent>();

	socketComp.isCutInChasing = true;

	SetLocalPlayerIdByMsg(msgPtr->playerids());

	socketComp.curServerFrameId = msgPtr->cmds().size() - 1;

	//����׷֡��Ϣ
	PlayersCommand cmd;

	for (auto& cmdData : msgPtr->cmds()) {

		cmd.frameId = cmdData.frameid();
		cmd.commandArray = std::array<BUTTON::Type, PLAYER_NUM>{};

		const auto& playersCmd = cmdData.playercmds();

		//�����û�����ָ��
		for (int i = 0; i < playersCmd.size(); ++i) {

			cmd.commandArray[i] = static_cast<BUTTON::Type>(playersCmd.Get(i));

		}

		socketComp.playersCmdsBuffer.push_back(cmd);

	}
}

void SocketSystem::SetLocalPlayerIdByMsg(const google::protobuf::RepeatedPtrField<XTankMsg::PlayerId>& playerIds)
{

	auto& socketComp = mWorld->GetSingletonComponent<SocketComponent>();

	//ȷ����������ڷ����е�id
	std::string localIP = SocketClient::Instance().GetLocalIP();
	unsigned long pid = GetCurrentProcessId();

	for (int i = 0; i < playerIds.size(); ++i) {
		auto& playerId = playerIds.Get(i);
		if (localIP == playerId.ip() && pid == playerId.pid()) {
			socketComp.localPlayerId = i;
			break;
		}
	}
}

