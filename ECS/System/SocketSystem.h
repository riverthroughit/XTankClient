#pragma once

#include "ECS/System.h"
#include "Socket/MessageQueue.h"
#include <memory>

class SocketSystem :public System {

	using Message = google::protobuf::Message;

public:
	virtual void Init() override;
	virtual void Tick(float dt)override;
private:
	//获取服务器消息
	MessageData ReceiveMsg();

	//更新用户命令
	void UpdatePlayersCmd(const MessageData& msgData);
};