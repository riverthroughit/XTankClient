#pragma once

#include "ECS/System.h"
#include "Socket/MessageQueue.h"
#include <memory>

class SocketSystem :public System<SocketSystem> {

	using Message = google::protobuf::Message;

public:
	virtual void Init() override;
	virtual void Tick(float dt)override;
	
	//追帧时的更新 不会向服务器发送本地操作
	void TickInChasing(float dt);

	//等待服务器下发开赛通知或追帧通知
	void WaitStart();

	//是否中途加入
	bool NeedChasing();

	//是否追帧结束
	bool IsChasingEnd();

private:
	//获取服务器消息
	MessageData ReceiveMsg();

	//更新用户命令
	void UpdatePlayersCmd(const MessageData& msgData);

	//追帧时更新用户命令
	void UpdatePlayersCmdInChasing(const MessageData& msgData);

	//发送本地玩家命令
	void SendLocalPlayerCmd();

	//将追帧信息等保存
	void SetCutInData(const MessageData& msgData);
};