#pragma once

#include "Config.h"

struct FrameComponent {

	//�������


	//��ǰ֡id
	int frameId{-1};
	//��ǰ֡�ڵ�ʱ��
	float frameTime{ LOCKSTEP_TICK };
	//��ǰ֡����
	float percent{};
	//�Ƿ񾭹�һ���߼�֡����
	bool isNeedTick{};
};