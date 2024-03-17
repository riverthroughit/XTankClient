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

	clock::time_point preTime{};
	clock::time_point curTime{};

	bool isStart{};

public:
	TickUtil() {
		Reset();
	}

	void Reset() {
		frameTime = tickTime;
		frameId = -1;
		percent = 0;
		isStart = false;
		preTime = curTime = clock::now();
	}

	void SetTickTime(float val) {
		tickTime = val;
		Reset();
	}


	void Tick() {

		if (!isStart) {
			isStart = true;
			preTime = clock::now();
		}

		curTime = clock::now();
		duration dtn = std::chrono::duration_cast<duration>(curTime - preTime);
		preTime = curTime;
		dt = dtn.count();
		frameTime += dt;

		if (frameTime > tickTime) {
			int dframe = frameTime / tickTime;
			frameTime -= dframe * tickTime;
			frameId += dframe;
		}

		percent = frameTime / tickTime;
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