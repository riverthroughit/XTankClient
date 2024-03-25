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

	//接受当前已到达的所有命令

	while (1) {
		MessageData msgData = MsgRecvQueue::Instance().GetAndPopTopMsg();
		if (msgData.type == XTankMsg::NONE) {
			break;
		}
		else if (msgData.type == XTankMsg::GAME_FORWARD_NTF) {
			PlayersCommand cmd = GetCmdFromMsgData(msgData);

			//防止收到重复命令 (追帧时)
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
		//之前的命令还未被执行 不更新
		return;
	}

	if (socketComp.playersCmdsBuffer.empty()) {
		//当前帧没有收到服务器转发指令
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

	//限制每帧只能发送一次 且中途加入时不可发送
	if (socketComp.lastSendFrameId < clientFrameId && !socketComp.isCutInChasing) {
		
		socketComp.lastSendFrameId = clientFrameId;

		auto& inputComp = mWorld->GetSingletonComponent<InputComponent>();
		MsgSendQueue::Instance().SendPlayerInputNtf(clientFrameId, inputComp.curBtn);
	}

}

void SocketSystem::TrySendChaseUpNtf()
{
	auto& socketComp = mWorld->GetSingletonComponent<SocketComponent>();

	//追帧完成 发送通知  
	if (socketComp.isCutInChasing && IsCmdBufferEmpty()) {

		socketComp.isCutInChasing = false;
		//通知服务器追帧完成
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

		socketComp.playersCmdsBuffer.push_back(cmd);

	}
}

void SocketSystem::SetExistPlayersByMsg(const google::protobuf::RepeatedPtrField<XTankMsg::PlayerId>& playerIds)
{

	auto& socketComp = mWorld->GetSingletonComponent<SocketComponent>();

	//确定本地玩家在房间中的id
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

	//根据玩家加入或退出修改玩家数组
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

