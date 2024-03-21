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

	//������Ƿ�Ϊ��
	bool IsCmdBufferEmpty();

	//�ȴ��������·�����֪ͨ��׷֪֡ͨ
	void WaitStart();

	//��ȡ�������Ϸ����֡�ʣ�׷֡ʱ����죩
	float GetTickTimeBasedOnChasing();

	//���µ�ǰ��Ҫִ�еķ���������
	void UpdateCurPlayersCmd();

	//�Ƿ�Ϊ��;���벢������׷֡
	bool IsCutInChasing();

	//����׷֡���֪ͨ
	void TrySendChaseUpNtf();

private:

	//��MessageData�н����õ�����
	PlayersCommand GetCmdFromMsgData(const MessageData& msgData);

	//���ͱ����������
	void SendLocalPlayerCmd();

	//��׷֡��Ϣ�ȱ���
	void InitCutInData(const MessageData& msgData);

	void SetLocalPlayerIdByMsg(const google::protobuf::RepeatedPtrField<XTankMsg::PlayerId>& playerIds);
};