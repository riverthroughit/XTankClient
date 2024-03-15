#pragma once

#include <chrono>
#include "Config.h"

struct FrameComponent {

	//�������


	using clock = std::chrono::high_resolution_clock;
	using duration = std::chrono::duration<float, std::milli>;

	//����
	float tickTime{ LOCKSTEP_TICK };
	//��ǰ֡id
	int frameId{-1};
	//��ǰ֡�ڵ�ʱ��
	float dt{ LOCKSTEP_TICK };
	//��ǰ֡����
	float percent{};
	//�Ƿ񾭹�һ���߼�֡����
	bool isNeedTick{};

	clock::time_point preTime{};
	clock::time_point curTime{};

	bool isStart{};

};