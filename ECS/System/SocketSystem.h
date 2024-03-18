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

	//等待服务器下发开赛通知或追帧通知
	void WaitStart();

	//是否中途加入
	bool NeedChasing();

	//是否追帧结束
	bool IsChasingEnd();

	//获取建议的游戏更新帧率（追帧时会更快）
	float GetTickTimeBasedOnChasing();

private:
	//获取服务器消息
	MessageData ReceiveMsg();

	//从MessageData中解析得到命令
	PlayersCommand GetCmdFromMsgData(const MessageData& msgData);

	//更新用户命令
	void UpdateCurPlayersCmd();

	//发送本地玩家命令
	void SendLocalPlayerCmd();

	//将追帧信息等保存
	void InitCutInData(const MessageData& msgData);
};