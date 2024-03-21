#pragma once

#include "ECS/System.h"
#include <chrono>


class FrameSystem :public System<FrameSystem> {

public:
	virtual void Tick(float = 0) override;

	virtual void Init()override;

	//����һ�η�����֡id
	void AddServerFrameId();

	//���±����߼�֡ʱ��
	void TickClientFrame();

	//�����߼�֡�Ƿ�ɸ���
	bool NeedClientTick();

	//�����߼�֡���
	float GetClientTickDt();

	//����һ�������߼�֡��ʱ��
	float GetTimeToNextClientFrame();
};