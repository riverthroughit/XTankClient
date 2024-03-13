#pragma once

#include "ECS/System.h"
#include "Socket/MessageQueue.h"
#include <memory>

class SocketSystem :public System<SocketSystem> {

	using Message = google::protobuf::Message;

public:
	virtual void Init() override;
	virtual void Tick(float dt)override;
private:
	//获取服务器消息
	void ParseMsg();

	//更新用户命令
	void UpdatePlayersCmd(const MessageData& msgData);

	//发送本地玩家命令
	void SendLocalPlayerCmd();
};