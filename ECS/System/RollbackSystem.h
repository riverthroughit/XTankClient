#pragma once

#include "ECS/System.h"
#include "TypeConfig.h"

class RollbackSystem:public System {

public:

	virtual void Tick(float dt)override;

private:

	//���µ�ǰ�õ���׼ȷ����
	void UpdatePreciseCmd();

	//���±����û�����
	void UpdateLocalCmd();

	//������һ�º�ع�����ȷ״̬
	void Rollback();

	//����Ԥ������
	PlayersCommand CreatePredictedCmdFrom(const PlayersCommand& cmd);

	//��֮ǰ����������͵�ǰ�ı�������Ԥ��
	void RunAhead();

};