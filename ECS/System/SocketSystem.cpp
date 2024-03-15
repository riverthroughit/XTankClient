#include "SocketSystem.h"
#include "ECS/Component/SocketComponent.h"
#include "Socket/MessageQueue.h"
#include "Socket/SocketClient.h"
#include "ECS/World.h"
#include "Protobuf/XTankMsg.pb.h"
#include <windows.h>
#include "ECS/Component/InputComponent.h"
#include "ECS/Component/FrameComponent.h"

using google::protobuf::Message;

void SocketSystem::Init()
{
	auto& socketComp = mWorld->GetSingletonComponent<SocketComponent>();
	
	//等待直到服务器发来开赛通知
	MessageData msgData = MsgRecvQueue::Instance().GetAndPopTopMsg();
	while (msgData.type != XTankMsg::GAME_START_NTF) {
		msgData = MsgRecvQueue::Instance().GetAndPopTopMsg();
	}

	std::shared_ptr<XTankMsg::GameStartNtf> msgPtr = std::static_pointer_cast<XTankMsg::GameStartNtf>(msgData.msgPtr);
	const auto& playerIds = msgPtr->playerids();
	socketComp.playerNum = playerIds.size();

	std::string localIP = SocketClient::Instance().GetLocalIP();
	unsigned long pid = GetCurrentProcessId();

	//确定本地玩家在房间中的id
	for (int i = 0; i < socketComp.playerNum; ++i) {
		auto& playerId = playerIds.Get(i);
		if (localIP == playerId.ip() && pid == playerId.pid()) {
			socketComp.localPlayerId = i;
			break;
		}
	}

	////延迟执行 等待其他客户端
	//Sleep(msgPtr->latency());

}

void SocketSystem::Tick(float dt)
{
	auto& socketComp = mWorld->GetSingletonComponent<SocketComponent>();
	socketComp.hasNewCmdMsg = false;
	//收
	ParseMsg();
	//发
	SendLocalPlayerCmd();
}

void SocketSystem::ParseMsg()
{
	MessageData msgData = MsgRecvQueue::Instance().GetAndPopTopMsg();

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


void SocketSystem::UpdatePlayersCmd(const MessageData& msgData)
{
	std::shared_ptr<XTankMsg::GameForwardNtf> msgPtr = std::static_pointer_cast<XTankMsg::GameForwardNtf>(msgData.msgPtr);
	auto& socketComp = mWorld->GetSingletonComponent<SocketComponent>();

	//更新socketComponent中的数据
	socketComp.hasNewCmdMsg = true;
	socketComp.curPlayersCmd.frameId = msgPtr->frameid();

	const auto& playersCmd = msgPtr->playercmds();

	//更新用户操作指令
	for (int i = 0; i < playersCmd.size(); ++i) {

		socketComp.curPlayersCmd.commandArray[i] = static_cast<BUTTON::Type>(playersCmd.Get(i));

	}
}

void SocketSystem::SendLocalPlayerCmd()
{

	auto& inputComp = mWorld->GetSingletonComponent<InputComponent>();

	auto& frameComp = mWorld->GetSingletonComponent<FrameComponent>();

	MsgSendQueue::Instance().SendPlayerInputNtf(frameComp.frameId, inputComp.curBtn);

}
