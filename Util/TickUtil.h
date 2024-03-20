#pragma once

#include <chrono>

class TickUtil {

	using clock = std::chrono::high_resolution_clock;
	using duration = std::chrono::duration<float, std::milli>;

	//周期
	float tickTime{};
	//当前帧id
	int frameId{ -1 };
	//当前帧内的时间
	float frameTime{};
	//当前帧进度
	float percent{};
	//间隔时间
	float dt{};
	//是否经过一帧 需要更新
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

	//设置一次延迟
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