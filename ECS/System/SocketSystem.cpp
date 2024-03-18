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

	std::string localIP = SocketClient::Instance().GetLocalIP();
	unsigned long pid = GetCurrentProcessId();

	//ȷ����������ڷ����е�id
	for (int i = 0; i < socketComp.playerNum; ++i) {
		auto& playerId = playerIds.Get(i);
		if (localIP == playerId.ip() && pid == playerId.pid()) {
			socketComp.localPlayerId = i;
			break;
		}
	}

}

void SocketSystem::Tick(float dt)
{
	auto& socketComp = mWorld->GetSingletonComponent<SocketComponent>();
	//��
	
	//���ܵ�ǰ�ѵ������������
	while (1) {
		MessageData msgData = ReceiveMsg();
		if (msgData.type == XTankMsg::NONE) {
			break;
		}
		else if (msgData.type == XTankMsg::GAME_FORWARD_NTF) {
			PlayersCommand cmd = GetCmdFromMsgData(msgData);

			//��ֹ�յ��ظ����� (׷֡ʱ)
			assert(cmd.frameId >= socketComp.curServerFrameId);
			socketComp.playersCmdsBuffer.push_back(cmd);

		}
	}

	UpdateCurPlayersCmd();

	//׷֡ʱ�����ͱ�����Ҳ���
	if (IsChasingEnd()) {

		//�Ƿ�Ϊ��;���뿪��׷֡
		if (!socketComp.isCutInChasing) {
			SendLocalPlayerCmd();
		}
		else {
			socketComp.isCutInChasing = false;
			//֪ͨ������׷֡���
			MsgSendQueue::Instance().SendPlayerChaseUpNtf();
		}	
	}
	
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

MessageData SocketSystem::ReceiveMsg()
{
	return MsgRecvQueue::Instance().GetAndPopTopMsg();

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
	socketComp.playersCmdsBuffer.pop_front();
	++socketComp.curServerFrameId;

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

	auto& inputComp = mWorld->GetSingletonComponent<InputComponent>();

	auto& frameComp = mWorld->GetSingletonComponent<FrameComponent>();

	MsgSendQueue::Instance().SendPlayerInputNtf(frameComp.frameId, inputComp.curBtn);

}

void SocketSystem::InitCutInData(const MessageData& msgData)
{

	auto msgPtr = std::static_pointer_cast<XTankMsg::PlayerCutInNtf>(msgData.msgPtr);
	auto& socketComp = mWorld->GetSingletonComponent<SocketComponent>();

	socketComp.isCutInChasing = true;

	//ȷ����������ڷ����е�id
	std::string localIP = SocketClient::Instance().GetLocalIP();
	unsigned long pid = GetCurrentProcessId();
	auto& playerIds = msgPtr->playerids();
	
	for (int i = 0; i < playerIds.size(); ++i) {
		auto& playerId = playerIds.Get(i);
		if (localIP == playerId.ip() && pid == playerId.pid()) {
			socketComp.localPlayerId = i;
			break;
		}
	}


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

