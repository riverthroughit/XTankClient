#pragma once

#include "ECS/System.h"
#include "TypeConfig.h"

class XTankWorld;

class RollbackSystem:public System<RollbackSystem> {

public:

	virtual void Tick(float dt)override;

	void Init()override;

	//�������粢���� ������ʵ������º����
	XTankWorld* DuplicateWorld();

	//����Ԥ�������
	PlayersCommand GetPredictedCmd();

private:

	//���·��������ص�׼ȷ����
	void UpdatePreciseCmd();

	//����Ԥ�������
	void UpdatePredictCmd();

	//����Ԥ�������
	void JudgeDuplicateWorld();

};