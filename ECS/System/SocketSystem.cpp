#include "SocketSystem.h"
#include "ECS/Component/SocketComponent.h"
#include "Socket/MessageQueue.h"
#include "Socket/SocketClient.h"
#include "ECS/World.h"
#include "Protobuf/XTankMsg.pb.h"
#include <windows.h>

using google::protobuf::Message;

void SocketSystem::Init()
{
	MessageData msgData = ReceiveMsg();
	std::shared_ptr<XTankMsg::GameStartNtf> msgPtr = std::static_pointer_cast<XTankMsg::GameStartNtf>(msgData.msgPtr);
	const auto& playerIds = msgPtr->playerids();

	std::string localIP = SocketClient::Instance().GetLocalIP();
	unsigned long pid = GetCurrentProcessId();

	auto& socketComp = mWorld->GetSingletonComponent<SocketComponent>();

	//确定本地玩家在房间中的id
	for (int i = 0; i < PLAYER_NUM; ++i) {
		auto& playerId = playerIds.Get(i);
		if (localIP == playerId.ip() && pid == playerId.pid()) {
			socketComp.localPlayerId = i;
		}

		if (!playerId.ip().empty()) {
			++socketComp.playerNum;
		}
	}
}

void SocketSystem::Tick(float dt)
{
	MessageData msgData = ReceiveMsg();

	switch (msgData.type)
	{
	case XTankMsg::NONE:

		break;

	case XTankMsg::GAME_FORWARD_NTF:
		UpdatePlayersCmd(msgData);
		break;

	default:
		break;
	}
}

MessageData SocketSystem::ReceiveMsg()
{
	MessageQueue& msgQue = MessageQueue::Instance();
	return msgQue.GetAndPopTopMsg();
}


void SocketSystem::UpdatePlayersCmd(const MessageData& msgData)
{
	std::shared_ptr<XTankMsg::GameForwardNtf> msgPtr = std::static_pointer_cast<XTankMsg::GameForwardNtf>(msgData.msgPtr);
	auto& socketComp = mWorld->GetSingletonComponent<SocketComponent>();

	//更新socketComponent中的数据
	socketComp.hasNewCmdMsg = true;
	socketComp.curPlayersCmd.frameId = msgPtr->frameid();

	const auto& playersCmd = msgPtr->playercmds();

	//更新用户操作指令
	for (int i = 0; i < PLAYER_NUM; ++i) {
		socketComp.curPlayersCmd.commandArray[i] = static_cast<BUTTON::Type>(playersCmd.Get(i));
	}
}
