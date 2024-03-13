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
	//��ȡ��������Ϣ
	void ParseMsg();

	//�����û�����
	void UpdatePlayersCmd(const MessageData& msgData);

	//���ͱ����������
	void SendLocalPlayerCmd();
};