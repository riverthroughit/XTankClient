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
	//��ȡ��������Ϣ
	MessageData ReceiveMsg();

	//�����û�����
	void UpdatePlayersCmd(const MessageData& msgData);
};