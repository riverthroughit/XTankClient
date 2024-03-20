#pragma once

#include <chrono>

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
	TickUtil(float tkTime) {
		tickTime = tkTime;
		Reset();
	}

	void Reset() {
		frameTime = tickTime;
		frameId = -1;
		percent = 0;
		isStart = false;

	}

	void SetTickTime(float val) {

		tickTime = val;

	}


	void Tick() {

		if (!isStart) {
			isStart = true;
			preTime = clock::now();
		}

		needTick = false;
		curTime = clock::now();
		duration dtn = std::chrono::duration_cast<duration>(curTime - preTime);
		preTime = curTime;
		frameTime += dtn.count();
		dt = frameTime;

		if (frameTime > tickTime) {
			needTick = true;
			int dframe = frameTime / tickTime;
			frameTime -= dframe * tickTime;
			frameId += dframe;
		}

		percent = frameTime / tickTime;
	}

	bool NeedTick() {
		return needTick;
	}

	//����һ���ӳ�
	void SetDelay(float val) {
		frameTime -= val;
	}

	int GetFrameId() {
		return frameId;
	}

	float GetPercent() {
		return percent;
	}

	float GetDt() {
		return dt;
	}

};