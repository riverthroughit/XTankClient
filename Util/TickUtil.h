#pragma once

#include <chrono>
#include <cassert>

class TickUtil {

	using clock = std::chrono::high_resolution_clock;
	using duration = std::chrono::duration<float, std::milli>;

	//����
	float tickTime{};
	//��ǰ֡id
	int frameId{ -1 };
	//��ǰ֡�ڵ�ʱ��
	float frameTime{};
	//��ǰ֡����
	float percent{};
	//���ʱ��
	float dt{};
	//�Ƿ񾭹�һ֡ ��Ҫ����
	bool needTick{};

	clock::time_point preTime{};
	clock::time_point curTime{};

	bool isStart{};

public:
	TickUtil(float tkTime);

	void Reset();

	void SetTickTime(float val);


	void Tick();

	bool NeedTick();

	float TimeToNextFrame();

	//����һ���ӳ�
	void SetDelay(float val);

	int GetFrameId();

	float GetPercent();

	float GetDt();

	float GetTickTime();

};