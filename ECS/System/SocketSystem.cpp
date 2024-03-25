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
#include <algorithm>

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

	SetExistPlayersByMsg(playerIds);

}

void SocketSystem::Tick(float dt)
{
	ReceiveCmd();
	SendLocalPlayerCmd();
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

}

bool SocketSystem::IsCmdBufferEmpty()
{
	auto& socketComp = mWorld->GetSingletonComponent<SocketComponent>();
	return socketComp.playersCmdsBuffer.empty();
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

	if (socketComp.hasCurCmd == true) {
		//֮ǰ�����δ��ִ�� ������
		return;
	}

	if (socketComp.playersCmdsBuffer.empty()) {
		//��ǰ֡û���յ�������ת��ָ��
		socketComp.hasCurCmd = false;
		socketComp.curPlayersCmd = {};
		return;
	}

	socketComp.hasCurCmd = true;
	socketComp.curPlayersCmd = socketComp.playersCmdsBuffer.front();
	socketComp.curServerFrameId = socketComp.curPlayersCmd.frameId;
	socketComp.playersCmdsBuffer.pop_front();


}

bool SocketSystem::IsCutInChasing()
{
	auto& socketComp = mWorld->GetSingletonComponent<SocketComponent>();

	return socketComp.isCutInChasing;
}

void SocketSystem::SendLocalPlayerCmd()
{


	auto& frameComp = mWorld->GetSingletonComponent<FrameComponent>();
	int clientFrameId = frameComp.clientTick.GetFrameId();

	auto& socketComp = mWorld->GetSingletonComponent<SocketComponent>();

	//����ÿֻ֡�ܷ���һ�� ����;����ʱ���ɷ���
	if (socketComp.lastSendFrameId < clientFrameId && !socketComp.isCutInChasing) {
		
		socketComp.lastSendFrameId = clientFrameId;

		auto& inputComp = mWorld->GetSingletonComponent<InputComponent>();
		MsgSendQueue::Instance().SendPlayerInputNtf(clientFrameId, inputComp.curBtn);
	}

}

void SocketSystem::TrySendChaseUpNtf()
{
	auto& socketComp = mWorld->GetSingletonComponent<SocketComponent>();

	//׷֡��� ����֪ͨ  
	if (socketComp.isCutInChasing && IsCmdBufferEmpty()) {

		socketComp.isCutInChasing = false;
		//֪ͨ������׷֡���
		MsgSendQueue::Instance().SendPlayerChaseUpNtf();
	}
}

void SocketSystem::InitCutInData(const MessageData& msgData)
{

	auto msgPtr = std::static_pointer_cast<XTankMsg::PlayerCutInNtf>(msgData.msgPtr);
	auto& socketComp = mWorld->GetSingletonComponent<SocketComponent>();

	socketComp.isCutInChasing = true;

	SetExistPlayersByMsg(msgPtr->playerids());

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

void SocketSystem::SetExistPlayersByMsg(const google::protobuf::RepeatedPtrField<XTankMsg::PlayerId>& playerIds)
{

	auto& socketComp = mWorld->GetSingletonComponent<SocketComponent>();

	//ȷ����������ڷ����е�id
	std::string localIP = SocketClient::Instance().GetLocalIP();
	unsigned long pid = GetCurrentProcessId();

	for (int i = 0; i < playerIds.size(); ++i) {

		auto& playerId = playerIds.Get(i);
		socketComp.existPlayers[i] = !playerId.ip().empty();

		if (localIP == playerId.ip() && pid == playerId.pid()) {
			socketComp.localPlayerId = i;
		}
	}
}

void SocketSystem::UpdateExistPlayersByCmd(const PlayersCommand& cmd)
{

	auto& socketComp = mWorld->GetSingletonComponent<SocketComponent>();

	//������Ҽ�����˳��޸��������
	for (int i = 0; i < cmd.commandArray.size(); ++i) {

		BUTTON::Type cmdType = cmd.commandArray[i];

		switch (cmdType)
		{
		case BUTTON::CUT_IN:
			
			for (int i = 0; i < socketComp.existPlayers.size(); ++i) {
				if (!socketComp.existPlayers[i]) {
					socketComp.existPlayers[i] = true;
				}
			}

			break;

		case BUTTON::EXIT:
			
			socketComp.existPlayers[i] = false;

			break;

		default:
			break;
		}
	}

}

