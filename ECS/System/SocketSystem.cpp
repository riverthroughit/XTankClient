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
	
	//等待直到服务器发来初始化通知
	MessageData msgData = MsgRecvQueue::Instance().GetAndPopTopMsg();
	while (msgData.type != XTankMsg::GAME_INIT_NTF) {
		msgData = MsgRecvQueue::Instance().GetAndPopTopMsg();
	}

	std::shared_ptr<XTankMsg::GameInitNtf> msgPtr = std::static_pointer_cast<XTankMsg::GameInitNtf>(msgData.msgPtr);
	
	//设置随机数
	auto& randComp = mWorld->GetSingletonComponent<RandComponent>();
	randComp.seed = msgPtr->randseed();

	
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

}

void SocketSystem::Tick(float dt)
{
	auto& socketComp = mWorld->GetSingletonComponent<SocketComponent>();
	socketComp.hasNewCmdMsg = false;
	//收
	MessageData msgData = ReceiveMsg();

	if (msgData.type == XTankMsg::GAME_FORWARD_NTF) {
		UpdatePlayersCmd(msgData);
	}

	//发
	SendLocalPlayerCmd();
}

void SocketSystem::TickInChasing(float dt)
{
	MessageData msgData = ReceiveMsg();
	if (msgData.type == XTankMsg::GAME_FORWARD_NTF) {
		UpdatePlayersCmdInChasing(msgData);
	}

	auto& socketComp = mWorld->GetSingletonComponent<SocketComponent>();
	socketComp.hasNewCmdMsg = true;
	socketComp.curPlayersCmd = socketComp.chasingCmds[socketComp.chasingFrameId];
	++socketComp.chasingFrameId;
}

void SocketSystem::WaitStart()
{
	//已准备完毕
	MsgSendQueue::Instance().SendGameReadyAck();

	MessageData msgData;
	
	while (1) {

		msgData = MsgRecvQueue::Instance().GetAndPopTopMsg();
		
		if (msgData.type == XTankMsg::GAME_START_NTF) {
			//开赛通知
			break;
		}
		else if (msgData.type == XTankMsg::PLAYER_CUT_IN_NTF) {
			//该玩家为中途加入 获取其房间内id以及追帧信息

			SetCutInData(msgData);

			break;

		}
	}
}

bool SocketSystem::NeedChasing()
{
	auto& socketComp = mWorld->GetSingletonComponent<SocketComponent>();

	return !socketComp.chasingCmds.empty();
}

bool SocketSystem::IsChasingEnd()
{
	auto& socketComp = mWorld->GetSingletonComponent<SocketComponent>();

	return socketComp.chasingFrameId >= socketComp.chasingCmds.size();
}

MessageData SocketSystem::ReceiveMsg()
{
	return MsgRecvQueue::Instance().GetAndPopTopMsg();

}


void SocketSystem::UpdatePlayersCmd(const MessageData& msgData)
{
	auto msgPtr = std::static_pointer_cast<XTankMsg::GameForwardNtf>(msgData.msgPtr);
	auto& socketComp = mWorld->GetSingletonComponent<SocketComponent>();

	//更新socketComponent中的数据
	socketComp.hasNewCmdMsg = true;
	auto& curPlayersCmd = socketComp.curPlayersCmd;
	curPlayersCmd.frameId = msgPtr->frameid();

	const auto& playersCmd = msgPtr->playercmds();

	//更新用户操作指令
	for (int i = 0; i < playersCmd.size(); ++i) {

		curPlayersCmd.commandArray[i] = static_cast<BUTTON::Type>(playersCmd.Get(i));
	}

	//判断是否有玩家加入指令 若有 则增加保存的玩家数量
	for (auto cmdType: curPlayersCmd.commandArray) {

		if (cmdType == BUTTON::CUT_IN) {
			++socketComp.playerNum;
		}
	}
}

void SocketSystem::UpdatePlayersCmdInChasing(const MessageData& msgData)
{
	auto& socketComp = mWorld->GetSingletonComponent<SocketComponent>();
	
	auto msgPtr = std::static_pointer_cast<XTankMsg::GameForwardNtf>(msgData.msgPtr);
	const auto& playersCmd = msgPtr->playercmds();
	
	PlayersCommand curCmd{};
	curCmd.frameId = msgPtr->frameid();
	//将新指令添加到追帧数组中
	for (int i = 0; i < curCmd.commandArray.size(); ++i) {

		curCmd.commandArray[i] = static_cast<BUTTON::Type>(playersCmd.Get(i));
	}

	socketComp.chasingCmds.push_back(curCmd);
}

void SocketSystem::SendLocalPlayerCmd()
{

	auto& inputComp = mWorld->GetSingletonComponent<InputComponent>();

	auto& frameComp = mWorld->GetSingletonComponent<FrameComponent>();

	MsgSendQueue::Instance().SendPlayerInputNtf(frameComp.frameId, inputComp.curBtn);

}

void SocketSystem::SetCutInData(const MessageData& msgData)
{

	auto msgPtr = std::static_pointer_cast<XTankMsg::PlayerCutInNtf>(msgData.msgPtr);
	auto& socketComp = mWorld->GetSingletonComponent<SocketComponent>();

	//确定本地玩家在房间中的id
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


	//保存追帧信息
	PlayersCommand cmd;

	for (auto& cmdData : msgPtr->cmds()) {

		cmd.frameId = cmdData.frameid();
		cmd.commandArray = std::array<BUTTON::Type, PLAYER_NUM>{};

		const auto& playersCmd = cmdData.playercmds();

		//更新用户操作指令
		for (int i = 0; i < playersCmd.size(); ++i) {

			cmd.commandArray[i] = static_cast<BUTTON::Type>(playersCmd.Get(i));

		}

		socketComp.chasingCmds.push_back(cmd);

	}
}

