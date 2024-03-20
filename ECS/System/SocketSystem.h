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

	//������Ϣ
	void ReceiveCmd();

	//������Ϣ
	void SendCmd();

	//�з������·����µ�����
	bool HasNewCmdMsg();

	//�ȴ��������·�����֪ͨ��׷֪֡ͨ
	void WaitStart();

	//�Ƿ���;����
	bool NeedChasing();

	//�Ƿ�׷֡����
	bool IsChasingEnd();

	//��ȡ�������Ϸ����֡�ʣ�׷֡ʱ����죩
	float GetTickTimeBasedOnChasing();

private:

	//��MessageData�н����õ�����
	PlayersCommand GetCmdFromMsgData(const MessageData& msgData);

	//�����û�����
	void UpdateCurPlayersCmd();

	//���ͱ����������
	void SendLocalPlayerCmd();

	//��׷֡��Ϣ�ȱ���
	void InitCutInData(const MessageData& msgData);

	void SetLocalPlayerIdByMsg(const google::protobuf::RepeatedPtrField<XTankMsg::PlayerId>& playerIds);
};