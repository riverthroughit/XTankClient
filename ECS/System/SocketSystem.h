#pragma once

#include "ECS/System.h"
#include "Socket/MessageQueue.h"
#include <memory>
#include "TypeConfig.h"

class SocketSystem :public System<SocketSystem> {

	using Message = google::protobuf::Message;

public:
	virtual void Init() override;
	virtual void Tick(float dt)override;

	//接收消息
	void ReceiveCmd();

	//命令缓冲是否为空
	bool IsCmdBufferEmpty();

	//等待服务器下发开赛通知或追帧通知
	void WaitStart();

	//获取建议的游戏更新帧率（追帧时会更快）
	float GetTickTimeBasedOnChasing();

	//更新当前需要执行的服务器命令
	void UpdateCurPlayersCmd();

	//是否为中途加入并且正在追帧
	bool IsCutInChasing();

	//发送追帧完成通知
	void TrySendChaseUpNtf();

private:

	//从MessageData中解析得到命令
	PlayersCommand GetCmdFromMsgData(const MessageData& msgData);

	//发送本地玩家命令
	void SendLocalPlayerCmd();

	//将追帧信息等保存
	void InitCutInData(const MessageData& msgData);

	void SetLocalPlayerIdByMsg(const google::protobuf::RepeatedPtrField<XTankMsg::PlayerId>& playerIds);
};