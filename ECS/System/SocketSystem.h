#pragma once

#include "ECS/System.h"
#include "Socket/MessageQueue.h"
#include <memory>

class SocketSystem :public System<SocketSystem> {

	using Message = google::protobuf::Message;

public:
	virtual void Init() override;
	virtual void Tick(float dt)override;
	
	//׷֡ʱ�ĸ��� ��������������ͱ��ز���
	void TickInChasing(float dt);

	//�ȴ��������·�����֪ͨ��׷֪֡ͨ
	void WaitStart();

	//�Ƿ���;����
	bool NeedChasing();

	//�Ƿ�׷֡����
	bool IsChasingEnd();

private:
	//��ȡ��������Ϣ
	MessageData ReceiveMsg();

	//�����û�����
	void UpdatePlayersCmd(const MessageData& msgData);

	//׷֡ʱ�����û�����
	void UpdatePlayersCmdInChasing(const MessageData& msgData);

	//���ͱ����������
	void SendLocalPlayerCmd();

	//��׷֡��Ϣ�ȱ���
	void SetCutInData(const MessageData& msgData);
};