#pragma once

#include "ECS/System.h"
#include "TypeConfig.h"
#include <vector>
#include <array>

class XTankWorld;

class RollbackSystem:public System<RollbackSystem> {

public:

	virtual void Tick(float dt)override;

	void Init()override;

	//��ȡ���Ƶ�����
	XTankWorld* GetDuplicateWorld();

	//����Ԥ������
	void TickPredictWorld(float dt);

private:

	//���·��������ص�׼ȷ����
	void UpdatePreciseCmd();

	//����Ԥ�������
	void UpdatePredictCmd();

	//��������Ԥ����һ����
	std::array<BUTTON::Type, PLAYER_NUM> GetPredictCmdFrom(const std::array<BUTTON::Type, PLAYER_NUM>& preCmd);

	//�ж�Ԥ���Ƿ�ɹ� �����ɹ� ���õ�ǰȨ����������Ԥ������ǰ״̬
	void RollbackPredictCmd();

	//Ԥ����һָ֡��
	void PredictNextCmd();

	//��ȡԤ��������ִ�е�����
	std::vector<PlayersCommand> GetPredictCmds();
};