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

	//û��Ԥ��ʧ�� ��������Ԥ������
	void RunAheadPredictWorld(float dt);

	//Ԥ��ʧ�� ��ع�Ԥ������
	void RollbackPredictWorld(float dt);

	//���Իع�Ԥ������ ��Ԥ��ɹ� ���ع� ��ʧ�� �ع�
	void TryRollbackPredictWorld(float dt);

	//���·��������ص�׼ȷ����
	void UpdatePreciseCmd();

	//�ж�Ԥ���Ƿ�ɹ� �����ɹ� ���õ�ǰȨ����������Ԥ������ǰ״̬
	void RollbackPredictCmd();

	//Ԥ����һָ֡��
	void PredictNextCmd();

private:

	//��������Ԥ����һ����
	std::array<BUTTON::Type, PLAYER_NUM> GetPredictCmdFrom(const std::array<BUTTON::Type, PLAYER_NUM>& preCmd);

	//��ǰ֡�Ƿ��Ѿ�Ԥ���
	bool HasPredictCurFrame();

	//�����Ƿ���Ҫ�ع���ȡԤ��������ִ�е�����
	std::vector<PlayersCommand> GetPredictCmdsByNeedRollback(bool needRollBack);
};