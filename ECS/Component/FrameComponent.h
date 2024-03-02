#pragma once

#include "Util/Macro.h"

struct FrameComponent {

	SINGLETON(FrameComponent);

	//��ǰ֡id
	unsigned int frameId{};
	//��ǰ֡�ڵ�ʱ��
	float frameTime{};
	//��ǰ֡����
	float percent{};
	//�Ƿ񾭹�һ���߼�֡����
	bool isNeedTick{};
};