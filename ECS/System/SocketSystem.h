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

	//�ȴ��������·�����֪ͨ��׷֪֡ͨ
	void WaitStart();

	//�Ƿ���;����
	bool NeedChasing();

	//�Ƿ�׷֡����
	bool IsChasingEnd();

	//��ȡ�������Ϸ����֡�ʣ�׷֡ʱ����죩
	float GetTickTimeBasedOnChasing();

private:
	//��ȡ��������Ϣ
	MessageData ReceiveMsg();

	//��MessageData�н����õ�����
	PlayersCommand GetCmdFromMsgData(const MessageData& msgData);

	//�����û�����
	void UpdateCurPlayersCmd();

	//���ͱ����������
	void SendLocalPlayerCmd();

	//��׷֡��Ϣ�ȱ���
	void InitCutInData(const MessageData& msgData);
};